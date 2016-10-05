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
		break;
	}
	default:
		res.handled = false;
		break;
	}

	return res;
}