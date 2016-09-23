#include "D2DTransformState.h"

D2DTransformState::D2DTransformState(ID2D1DeviceContext *d2dCtx)
	: d2dCtx(d2dCtx)
{
	this->d2dCtx->GetTransform(&this->transform);
}

D2DTransformState::~D2DTransformState() {
	this->d2dCtx->SetTransform(this->transform);
}

const D2D1_MATRIX_3X2_F &D2DTransformState::GetTransform() const {
	return this->transform;
}