#include "PlayerWindow.h"
#include "..\Dx\HwndOutput.h"

#include <libhelpers\Dx\Dx.h>

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
		DirectX::XMFLOAT2 size;

		size.x = (float)LOWORD(lparam);
		size.y = (float)HIWORD(lparam);

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
		res.handled = false;
		break;
	}

	return res;
}