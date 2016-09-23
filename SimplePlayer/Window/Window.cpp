#include "Window.h"

#include <libhelpers\HSystem.h>

Window::Window()
	: handle(NULL)
{
	WindowInitData initData;
	auto initFuture = initData.InitPromise.get_future();

	this->wndThread = std::thread([this, &initData]() {
		this->WndThreadMain(&initData);
	});

	initFuture.get();
}

Window::~Window() {
	// DestroyWindow won't work from another thread.
	// PostMessage works.
	// WindowMessages::Shutdown used to call DestroyWindow from window thread
	// DestroyWindow must(!) be called to clean window resources
	PostMessageW(this->handle, WM_USER, (uint32_t)WindowMessages::Shutdown, 0);

	if (this->wndThread.joinable()) {
		this->wndThread.join();
	}

	Window::RemoveThisMap(this->handle);
}

DirectX::XMUINT2 Window::GetSize() const {
	RECT rect;
	DirectX::XMUINT2 size;

	if (GetClientRect(this->handle, &rect)) {
		size.x = rect.right - rect.left;
		size.y = rect.bottom - rect.top;
	}
	else {
		size.x = size.y = 1;
	}

	return size;
}

void Window::Show() {
	auto tmp = ShowWindow(this->handle, SW_SHOWDEFAULT);
	int top = 234;
}

void Window::ProcessMessages() {
	MSG msg;

	while (PeekMessageW(&msg, this->handle, 0, 0, PM_REMOVE)) {
		DispatchMessageW(&msg);
	}
}

HWND Window::GetHwnd() const {
	return this->handle;
}

const std::wstring &Window::GetWndClassName() const {
	return this->className;
}

void Window::ProcessMsg(uint32_t msg, WPARAM wparam, LPARAM lparam) {

}

void Window::WndThreadMain(WindowInitData *initData) {
	bool initFailed = false;
	try {
		WNDCLASSW wndclass;
		auto hinst = GetModuleHandleW(NULL);
		this->className = L"Window" + std::to_wstring(Window::nextWndId++);

		wndclass.style = CS_DBLCLKS;
		wndclass.lpfnWndProc = Window::WndProcTmp;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = hinst;
		wndclass.hIcon = NULL;
		wndclass.hCursor = NULL;
		wndclass.hbrBackground = HBRUSH(COLOR_WINDOW + 1);
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName = this->className.data();

		if (!RegisterClassW(&wndclass)) {
			H::System::ThrowIfFailed(E_FAIL);
		}

		this->handle = CreateWindowW(
			this->className.data(),
			L"WindowTitle",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			NULL,
			NULL,
			hinst,
			NULL);

		if (!this->handle) {
			H::System::ThrowIfFailed(E_FAIL);
		}

		this->Show();

		// small hack
		// call AddThisMap after Show to use DefWindowProc from Window::WndProcTmp while base class initializing
		// TODO try to find better aproach without making more compicated code since current logic fits all needs
		Window::AddThisMap(this->handle, this);

		initData->InitPromise.set_value(true);
	}
	catch (...) {
		initData->InitPromise.set_exception(std::current_exception());
		initFailed = true;
	}
	initData = nullptr; // Must not be used after InitPromise.set_value/set_exception

	if (!initFailed) {
		MSG msg;
		BOOL ret;

		while ((ret = GetMessageW(&msg, this->handle, 0, 0)) != 0) {
			if (ret == -1) {
				// handle the error and possibly exit
				break;
			}
			else {
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
		}
	}
}

LRESULT Window::WndProc(uint32_t msg, WPARAM wparam, LPARAM lparam) {
	this->ProcessMsg(msg, wparam, lparam);

	switch (msg) {
	case WM_USER: {
		auto msg = (WindowMessages)wparam;
		switch (msg) {
		case WindowMessages::Shutdown:
			DestroyWindow(this->handle);
			break;
		default:
			break;
		}
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(this->handle, msg, wparam, lparam);
	}

	return 0L;
}




std::atomic_uint64_t Window::nextWndId(0);
thread::critical_section Window::thisMapCs;
std::map<HWND, Window *> Window::thisMap;

LRESULT CALLBACK Window::WndProcTmp(HWND h, UINT msg, WPARAM wparam, LPARAM lparam) {
	auto _this = Window::GetThisMap(h);

	if (!_this) {
		// This can happen while window is initializing
		return DefWindowProc(h, msg, wparam, lparam);
	}

	// should be alway true
	H::System::Assert(h == _this->handle);

	return _this->WndProc(msg, wparam, lparam);
}

void Window::AddThisMap(HWND h, Window *_this) {
	thread::critical_section::scoped_lock lk(Window::thisMapCs);
	Window::thisMap[h] = _this;
}

Window *Window::GetThisMap(HWND h) {
	thread::critical_section::scoped_lock lk(Window::thisMapCs);
	return Window::thisMap[h];
}

void Window::RemoveThisMap(HWND h) {
	thread::critical_section::scoped_lock lk(Window::thisMapCs);
	Window::thisMap.erase(h);
}