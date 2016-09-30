#pragma once
#include "WindowBaseData.h"

#include <DirectXMath.h>

class Window {
public:
	Window(WindowBaseData &baseData);
	virtual ~Window();

	DirectX::XMUINT2 GetSize() const;

	void ProcessMessages();
	virtual void ProcessMsg(uint32_t msg, WPARAM wparam, LPARAM lparam);

protected:
	HWND GetHwnd() const;
	const std::wstring &GetWndClassName() const;

private:
	WindowBaseData &baseData;
};