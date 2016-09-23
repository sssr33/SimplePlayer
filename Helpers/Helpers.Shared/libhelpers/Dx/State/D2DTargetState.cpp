#include "D2DTargetState.h"

D2DTargetState::D2DTargetState(ID2D1DeviceContext *d2dCtx)
	: d2dCtx(d2dCtx)
{
	this->d2dCtx->GetDpi(&this->dpi.x, &this->dpi.y);
	this->d2dCtx->GetTarget(this->rt.GetAddressOf());
}

D2DTargetState::~D2DTargetState() {
	this->d2dCtx->SetDpi(this->dpi.x, this->dpi.y);
	this->d2dCtx->SetTarget(this->rt.Get());
}

const DirectX::XMFLOAT2 &D2DTargetState::GetDPI() const {
	return this->dpi;
}