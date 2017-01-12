#pragma once
#include "SwapChainPanelOutput.h"
#include "../Dx.h"
#include "../../Thread/critical_section.h"
#include "../../HSystem.h"
#include "../../HMath.h"

#include <memory>
#include <thread>
#include <type_traits>

enum class RenderThreadState {
	Work,
	Pause, // can be changed to Work to let thread continue and not exit
	Stop, // thread will exit very soon
};

template<class T>
class WinRtRenderer {
public:
	template<class... Args>
	WinRtRenderer(
		Windows::UI::Xaml::Controls::SwapChainPanel ^panel, Args&&... args)
		: output(&this->dxDev, panel), pointerMoves(false),
		renderer(&this->dxDev, &this->output, std::forward<Args>(args)...), renderThreadState(RenderThreadState::Stop)
	{
		auto window = Windows::UI::Xaml::Window::Current->CoreWindow;
		auto displayInformation = Windows::Graphics::Display::DisplayInformation::GetForCurrentView();

		window->VisibilityChanged += H::System::MakeTypedEventHandler(
			[=](Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::VisibilityChangedEventArgs ^args)
		{
			thread::critical_section::scoped_lock lk(this->renderThreadStateCs);

			if (!args->Visible) {
				this->renderThreadState = RenderThreadState::Pause;
			}
			else {
				if (this->renderThreadState == RenderThreadState::Stop) {
					if (this->renderThread.joinable()) {
						this->renderThread.join();
					}

					this->StartRenderThread();
				}
				else {
					this->renderThreadState = RenderThreadState::Work;
				}
			}
		});

		displayInformation->OrientationChanged += H::System::MakeTypedEventHandler(
			[=](Windows::Graphics::Display::DisplayInformation ^sender, Platform::Object ^args)
		{
			thread::critical_section::scoped_lock lk(this->cs);
			auto panel = this->output.GetSwapChainPanel();
			auto orientation = sender->CurrentOrientation;
			auto dpi = sender->LogicalDpi;
			auto scale = DirectX::XMFLOAT2(panel->CompositionScaleX, panel->CompositionScaleY);
			auto size = DirectX::XMFLOAT2((float)panel->ActualWidth, (float)panel->ActualHeight);

			this->TryResize(size, scale, dpi, orientation);
		});

		displayInformation->DisplayContentsInvalidated += H::System::MakeTypedEventHandler(
			[=](Windows::Graphics::Display::DisplayInformation ^sender, Platform::Object ^args)
		{
			int stop = 324;
		});

		displayInformation->DpiChanged += H::System::MakeTypedEventHandler(
			[=](Windows::Graphics::Display::DisplayInformation ^sender, Platform::Object ^args)
		{
			thread::critical_section::scoped_lock lk(this->cs);
			auto panel = this->output.GetSwapChainPanel();
			auto orientation = sender->CurrentOrientation;
			auto dpi = sender->LogicalDpi;
			auto scale = DirectX::XMFLOAT2(panel->CompositionScaleX, panel->CompositionScaleY);
			auto size = DirectX::XMFLOAT2((float)panel->ActualWidth, (float)panel->ActualHeight);

			this->TryResize(size, scale, dpi, orientation);
		});

		panel->CompositionScaleChanged += H::System::MakeTypedEventHandler(
			[=](Windows::UI::Xaml::Controls::SwapChainPanel ^sender, Platform::Object ^args)
		{
			auto displayInformation = Windows::Graphics::Display::DisplayInformation::GetForCurrentView();

			thread::critical_section::scoped_lock lk(this->cs);
			auto orientation = displayInformation->CurrentOrientation;
			auto dpi = displayInformation->LogicalDpi;
			auto scale = DirectX::XMFLOAT2(sender->CompositionScaleX, sender->CompositionScaleY);
			auto size = DirectX::XMFLOAT2((float)sender->ActualWidth, (float)sender->ActualHeight);

			this->TryResize(size, scale, dpi, orientation);
		});

		panel->SizeChanged += ref new Windows::UI::Xaml::SizeChangedEventHandler(
			[=](Platform::Object ^sender, Windows::UI::Xaml::SizeChangedEventArgs ^args)
		{
			auto displayInformation = Windows::Graphics::Display::DisplayInformation::GetForCurrentView();
			auto size = DirectX::XMFLOAT2(args->NewSize.Width, args->NewSize.Height);

			thread::critical_section::scoped_lock lk(this->cs);
			auto panel = this->output.GetSwapChainPanel();
			auto orientation = displayInformation->CurrentOrientation;
			auto dpi = displayInformation->LogicalDpi;
			auto scale = DirectX::XMFLOAT2(panel->CompositionScaleX, panel->CompositionScaleY);

			this->TryResize(size, scale, dpi, orientation);
		});

		this->renderer.OutputParametersChanged();

		this->StartRenderThread();
		this->inputThread = std::thread([=]() { this->Input(panel); });
	}

	~WinRtRenderer() {
		static_assert(std::is_base_of<IRenderer, T>::value, "Renderer must inherit from IRenderer");

		this->renderThreadState = RenderThreadState::Stop;
		this->coreInput->Dispatcher->StopProcessEvents();

		if (this->renderThread.joinable()) {
			this->renderThread.join();
		}

		if (this->inputThread.joinable()) {
			this->inputThread.join();
		}
	}

	T *operator->() {
		return &this->renderer;
	}

	T *get() {
		return &this->renderer;
	}

	std::function<void(Windows::UI::Input::PointerPoint^)> pointerPressed;
	std::function<void(Windows::UI::Input::PointerPoint^)> pointerMoved;
	std::function<void(Windows::UI::Input::PointerPoint^)> pointerReleased;
	std::function<void(Windows::UI::Input::PointerPoint^)> pointerWheelChanged;

	DirectX::XMFLOAT4 GetRTColor() const {
		return this->output.GetRTColor();
	}

	void SetRTColor(const DirectX::XMFLOAT4 &color) {
		this->output.SetRTColor(color);
	}

private:
	DxDevice dxDev;
	SwapChainPanelOutput output;
	T renderer;

	thread::critical_section cs;

	Windows::UI::Core::CoreIndependentInputSource ^coreInput;

	std::thread renderThread;
	std::thread inputThread;

	thread::critical_section renderThreadStateCs;
	RenderThreadState renderThreadState;

	bool pointerMoves;

	void Input(Windows::UI::Xaml::Controls::SwapChainPanel ^panel) {
		this->coreInput = panel->CreateCoreIndependentInputSource(
			Windows::UI::Core::CoreInputDeviceTypes::Mouse |
			Windows::UI::Core::CoreInputDeviceTypes::Touch |
			Windows::UI::Core::CoreInputDeviceTypes::Pen);

		this->coreInput->PointerPressed += H::System::MakeTypedEventHandler(
			[=](Platform::Object ^sender, Windows::UI::Core::PointerEventArgs ^args)
		{
			auto pt = args->CurrentPoint;
			this->pointerMoves = true;

			this->pointerPressed(pt);
		});
		this->coreInput->PointerMoved += H::System::MakeTypedEventHandler(
			[=](Platform::Object ^sender, Windows::UI::Core::PointerEventArgs ^args)
		{
			if (this->pointerMoves) {
				auto pt = args->CurrentPoint;
				this->pointerMoved(pt);
			}
		});
		this->coreInput->PointerReleased += H::System::MakeTypedEventHandler(
			[=](Platform::Object ^sender, Windows::UI::Core::PointerEventArgs ^args)
		{
			auto pt = args->CurrentPoint;
			this->pointerMoves = false;

			this->pointerReleased(pt);
		});
		this->coreInput->PointerWheelChanged += H::System::MakeTypedEventHandler(
			[=](Platform::Object ^sender, Windows::UI::Core::PointerEventArgs ^args)
		{
			auto pt = args->CurrentPoint;
			this->pointerWheelChanged(pt);
		});

		this->coreInput->Dispatcher->ProcessEvents(Windows::UI::Core::CoreProcessEventsOption::ProcessUntilQuit);
	}

	void Render() {
		while (this->CheckRenderThreadState()) {
			thread::critical_section::scoped_yield_lock lk(this->cs);

			this->output.BeginRender();
			this->renderer.Render();
			this->output.EndRender();
		}
	}

	bool CheckRenderThreadState() {
		bool continueWork = true;
		thread::critical_section::scoped_yield_lock lk(this->renderThreadStateCs);

		if (this->renderThreadState != RenderThreadState::Work) {
			continueWork = false;
			this->renderThreadState = RenderThreadState::Stop;
		}

		return continueWork;
	}

	void StartRenderThread() {
		this->renderThreadState = RenderThreadState::Work;
		this->renderThread = std::thread([=]() { this->Render(); });
	}

	// Need to inspect all parameters in order to use correct size parameters under windows 8.1
	// Because under 8.1 it isn't reports events about all changes, for example it refers to DPI
	void TryResize(
		const DirectX::XMFLOAT2 &newSize,
		const DirectX::XMFLOAT2 &newScale,
		float dpi,
		Windows::Graphics::Display::DisplayOrientations orientation)
	{
		bool needResize = false;
		auto oldOrientation = this->output.GetCurrentOrientation();
		auto oldDpi = this->output.GetLogicalDpi();
		auto oldScale = this->output.GetCompositionScale();
		auto oldSize = this->output.GetLogicalSize();

		if (orientation != oldOrientation) {
			needResize = true;
			this->output.SetCurrentOrientation(orientation);
		}

		if (dpi != oldDpi) {
			needResize = true;
			this->output.SetLogicalDpi(dpi);
		}

		if (newScale != oldScale) {
			needResize = true;
			this->output.SetCompositionScale(newScale);
		}

		if (newSize != oldSize) {
			needResize = true;
			this->output.SetLogicalSize(newSize);
		}

		if (needResize) {
			this->output.Resize();
			this->renderer.OutputParametersChanged();
		}
	}
};