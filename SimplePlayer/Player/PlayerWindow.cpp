#include "PlayerWindow.h"
#include "..\Dx\HwndOutput.h"

#include <libhelpers\Dx\Dx.h>
#include <libhelpers\HSystem.h>
#include <libhelpers\PointerWrappers.h>

PlayerWindow::PlayerWindow(WindowBaseData &baseData) 
	: Window(baseData)
{
}

PlayerWindow::~PlayerWindow() {
}

ProcessMsgResult PlayerWindow::ProcessMsg(uint32_t msg, WPARAM wparam, LPARAM lparam) {
	ProcessMsgResult res(true);

	switch (msg) {
	case WM_CREATE: {
		this->renderer = std::make_unique<HwndRenderer<PlayerRenderer>>(this->GetHwnd());
		break;
	}
	case WM_SIZE: {
		DirectX::XMFLOAT2 size = this->GetXMFLOAT2FromLParam(lparam);

		if (size.x == 0.0f || size.y == 0.0f) {
			this->renderer->PauseRendering();
		}
		else {
			this->renderer->ContinueRendering();
		}

		this->renderer->Resize(size);

		break;
	}
	default:
		do {
			res = this->ProcessInput(msg, wparam, lparam);
			if (res.handled) {
				break;
			}

		} while (false);
		break;
	}

	return res;
}

DirectX::XMFLOAT2 PlayerWindow::GetXMFLOAT2FromLParam(LPARAM lparam) {
	DirectX::XMFLOAT2 float2;

	float2.x = (float)LOWORD(lparam);
	float2.y = (float)HIWORD(lparam);

	return float2;
}

DirectX::XMFLOAT2 PlayerWindow::GetPointFromLParam(LPARAM lparam) {
	DirectX::XMFLOAT2 float2;

	// when using SetCapture+ReleaseCapture coordinates can be negative when pointer moves behind left-top
	float2.x = (float)(int16_t)LOWORD(lparam);
	float2.y = (float)(int16_t)HIWORD(lparam);

	return float2;
}

ProcessMsgResult PlayerWindow::ProcessInput(uint32_t msg, WPARAM wparam, LPARAM lparam) {
	ProcessMsgResult res(true);

	switch (msg) {
	case WM_LBUTTONDOWN: {
		auto pt = this->GetPointFromLParam(lparam);

		SetCapture(this->GetHwnd());

		this->inputWorker.AddTask(Unique(MakeGenericThreadTask([=]() {
			(*this->renderer)->PointerPressed(pt);
		})));
		break;
	}
	case WM_LBUTTONUP: {
		auto pt = this->GetPointFromLParam(lparam);

		ReleaseCapture();

		this->inputWorker.AddTask(Unique(MakeGenericThreadTask([=]() {
			(*this->renderer)->PointerReleased(pt);
		})));
		break;
	}
	case WM_MOUSEMOVE: {
		auto pt = this->GetPointFromLParam(lparam);

		this->inputWorker.AddTask(Unique(MakeGenericThreadTask([=]() {
			(*this->renderer)->PointerMoved(pt);
		})));
		break;
	}
	default:
		res.handled = false;
		break;
	}

	return res;
}