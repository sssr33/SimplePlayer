#pragma once
#include "..\DxDevice.h"
#include "libhelpers\Macros.h"

class D2DTransformState {
public:
	NO_COPY_MOVE(D2DTransformState);

	D2DTransformState(ID2D1DeviceContext *d2dCtx);
	~D2DTransformState();

	const D2D1_MATRIX_3X2_F &GetTransform() const;

private:
	ID2D1DeviceContext *d2dCtx;
	D2D1_MATRIX_3X2_F transform;
};