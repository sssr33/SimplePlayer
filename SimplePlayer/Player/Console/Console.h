#pragma once
#include "..\RenderContext.h"

class Console {
public:
	Console(DxDevice *dxDev);
	~Console();

	void Render(RenderContext &renderCtx);

private:
	DxDevice *dxDev;
};