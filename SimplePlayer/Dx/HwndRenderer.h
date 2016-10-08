#pragma once
#include "HwndOutput.h"

#include <thread>
#include <type_traits>
#include <libhelpers\Thread\critical_section.h>
#include <libhelpers\Thread\condition_variable.h>
#include <libhelpers\Dx\Renderer\IRenderer.h>

template<class T>
class HwndRenderer {
public:
	template<class... Args>
	HwndRenderer(HWND hwnd, Args&&... args)
		: output(&this->dxDev, hwnd),
		renderer(&this->dxDev, &this->output, std::forward<Args>(args)...),
		renderThreadState(RenderThreadState::Pause),
		newSize(0.0f, 0.0f), resizeRequested(false)
	{
		this->renderThread = std::thread([=]() { this->RenderProc(); });
	}

	~HwndRenderer() {
		static_assert(std::is_base_of<IRenderer, T>::value, "Renderer must inherit from IRenderer");

		this->renderThreadState = RenderThreadState::Stop;
		this->renderThreadStateCv.notify();

		if (this->renderThread.joinable()) {
			this->renderThread.join();
		}
	}

	T *operator->() {
		return &this->renderer;
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

private:
	enum class RenderThreadState {
		Work,
		Pause, // can be changed to Work to let thread continue and not exit
		Stop, // thread will exit very soon
	};

	DxDevice dxDev;
	HwndOutput output;
	T renderer;

	thread::critical_section cs;
	thread::condition_variable renderThreadStateCv;
	std::thread renderThread;
	RenderThreadState renderThreadState;

	DirectX::XMFLOAT2 newSize;
	bool resizeRequested;

	void RenderProc() {
		while (this->CheckRenderThreadState()) {
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
				this->output.SetLogicalSize(size);
				this->output.Resize();
				this->renderer.OutputParametersChanged();
			}

			this->output.BeginRender();
			this->renderer.Render();
			this->output.EndRender();
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