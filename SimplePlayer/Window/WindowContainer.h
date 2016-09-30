#pragma once
#include "WindowMain.h"

template<class T>
class WindowContainer {
public:
	WindowContainer()
		: window(baseData), winMain(baseData, window)
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