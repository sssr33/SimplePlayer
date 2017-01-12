#pragma once
#include <winapifamily.h>
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)

#include "ISurface9Access.h"
#include "Texture9On11Output.h"

#include <thread>

// http://stackoverflow.com/questions/2004820/inherit-interfaces-which-share-a-method-name
class ISurface9AccessAdapter : public ISurface9Access {
public:
	ISurface9AccessAdapter() {}
	virtual ~ISurface9AccessAdapter() {}

	void Resize(int width, int height) override {
		this->ISurface9Resize(width, height);
	}

	Surface9Data NextSurface() override {
		return this->ISurface9NextSurface();
	}

	virtual void ISurface9Resize(int width, int height) = 0;
	virtual Surface9Data ISurface9NextSurface() = 0;
};

template<class T>
class Texture9On11Renderer : public ISurface9AccessAdapter {
public:
	template<class... Args>
	Texture9On11Renderer(Args&&... args)
		: output(&this->dxDev),
		renderer(&this->dxDev, &this->output, std::forward<Args>(args)...),
		renderThreadState(RenderThreadState::Pause),
		newSize(0.0f, 0.0f), resizeRequested(false)
	{
		this->renderThread = std::thread([=]() { this->RenderProc(); });
	}

	virtual ~Texture9On11Renderer() {
		static_assert(std::is_base_of<IRenderer, T>::value, "Renderer must inherit from IRenderer");

		this->renderThreadState = RenderThreadState::Stop;
		this->renderThreadStateCv.notify();

		// stop wait for composition;
		this->output.NextComposition();

		if (this->renderThread.joinable()) {
			this->renderThread.join();
		}
	}

	T *operator->() {
		return &this->renderer;
	}

	DirectX::XMFLOAT4 GetRTColor() const {
		return this->output.GetRTColor();
	}

	void SetRTColor(const DirectX::XMFLOAT4 &color) {
		this->output.SetRTColor(color);
	}

	void ContinueRendering() {
		thread::critical_section::scoped_lock lk(this->cs);

		this->renderThreadState = RenderThreadState::Work;
		this->renderThreadStateCv.notify();
	}

	void PauseRendering() {
		thread::critical_section::scoped_lock lk(this->cs);

		this->renderThreadState = RenderThreadState::Pause;
	}

	void Resize(const DirectX::XMFLOAT2 &size) {
		thread::critical_section::scoped_lock lk(this->cs);

		this->resizeRequested = true;
		this->newSize = size;
	}

	void RenderFrame() {
		thread::critical_section::scoped_lock lk(this->csRender);
		bool resize = false;
		DirectX::XMFLOAT2 size;

		{
			thread::critical_section::scoped_lock lk(this->cs);
			if (this->resizeRequested) {
				resize = true;
				size = this->newSize;
				this->resizeRequested = false;
			}
		}

		if (resize) {
			auto dxLk = dxDev.LockCtxScoped();
			this->output.SetLogicalSize(size);
			this->output.Resize();
			this->renderer.OutputParametersChanged();
		}

		{
			auto dxLk = dxDev.LockCtxScoped();
			this->output.BeginRender();
		}

		this->renderer.Render();

		{
			auto dxLk = dxDev.LockCtxScoped();
			this->output.EndRender();
		}

		this->output.WaitForComposition();
	}

	// ISurface9AccessAdapter
	virtual void ISurface9Resize(int width, int height) override {
		this->Resize(DirectX::XMFLOAT2((float)width, (float)height));
	}


	virtual Surface9Data ISurface9NextSurface() override {
		return this->output.NextD3D9Surface();
	}

private:
	enum class RenderThreadState {
		Work,
		Pause, // can be changed to Work to let thread continue and not exit
		Stop, // thread will exit very soon
	};

	DxDevice dxDev;
	Texture9On11Output output;
	T renderer;

	thread::critical_section cs;
	thread::critical_section csRender;
	thread::condition_variable renderThreadStateCv;
	std::thread renderThread;
	RenderThreadState renderThreadState;

	DirectX::XMFLOAT2 newSize;
	bool resizeRequested;

	void RenderProc() {
		while (this->CheckRenderThreadState()) {
			this->RenderFrame();
		}
	}

	bool CheckRenderThreadState() {
		bool continueWork = true;
		thread::critical_section::scoped_yield_lock lk(this->cs);

		while (this->renderThreadState == RenderThreadState::Pause) {
			this->renderThreadStateCv.wait(this->cs);
		}

		if (this->renderThreadState == RenderThreadState::Stop) {
			continueWork = false;
		}

		return continueWork;
	}
};

#endif