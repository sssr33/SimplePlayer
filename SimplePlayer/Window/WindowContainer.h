#pragma once
#include "WindowMain.h"

template<class T>
class WindowContainer {
public:
	WindowContainer(const std::wstring &windowTitle)
		: window(baseData), winMain(baseData, window, windowTitle)
	{
	}

	T *operator->() {
		return &this->window;
	}

	const T *operator->() const {
		return &this->window;
	}

private:
	WindowBaseData baseData;
	T window;
	WindowMain winMain;
};