#pragma once

#include <thread>
#include <string>
#include <windows.h>

class WindowBaseData {
public:
	HWND handle;
	std::wstring className;
	std::thread wndThread;

	WindowBaseData();
};