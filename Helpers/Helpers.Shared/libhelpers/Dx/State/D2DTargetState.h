#pragma once
#include "..\DxDevice.h"
#include "libhelpers\Macros.h"

class D2DTargetState {
public:
	NO_COPY(D2DTargetState);

	D2DTargetState(ID2D1DeviceContext *d2dCtx);
	D2DTargetState(D2DTargetState &&other);
	~D2DTargetState();

	D2DTargetState &operator=(D2DTargetState &&other);

	const DirectX::XMFLOAT2 &GetDPI() const;

private:
	ID2D1DeviceContext *d2dCtx;

	DirectX::XMFLOAT2 dpi;
	Microsoft::WRL::ComPtr<ID2D1Image> rt;
};