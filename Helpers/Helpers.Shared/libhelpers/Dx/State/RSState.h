#pragma once
#include "..\DxDevice.h"
#include "libhelpers\Macros.h"

class RSState {
public:
	NO_COPY(RSState);

	RSState(ID3D11DeviceContext *d3dCtx);
	RSState(RSState &&other);
	~RSState();

	RSState &operator=(RSState &&other);

private:
	ID3D11DeviceContext *d3dCtx;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rsState;
};