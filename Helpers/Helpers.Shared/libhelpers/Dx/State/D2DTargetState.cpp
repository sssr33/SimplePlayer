#include "D2DTargetState.h"

D2DTargetState::D2DTargetState(ID2D1DeviceContext *d2dCtx)
	: d2dCtx(d2dCtx)
{
	this->d2dCtx->GetDpi(&this->dpi.x, &this->dpi.y);
	this->d2dCtx->GetTarget(this->rt.GetAddressOf());
}

D2DTargetState::D2DTargetState(D2DTargetState &&other) 
	: d2dCtx(std::move(other.d2dCtx)), dpi(std::move(other.dpi)),
	rt(std::move(other.rt))
{
	other.d2dCtx = nullptr;
}

D2DTargetState::~D2DTargetState() {
	if (this->d2dCtx) {
		this->d2dCtx->SetDpi(this->dpi.x, this->dpi.y);
		this->d2dCtx->SetTarget(this->rt.Get());
	}
}

D2DTargetState &D2DTargetState::operator=(D2DTargetState &&other) {
	if (this != &other) {
		this->d2dCtx = std::move(other.d2dCtx);
		this->dpi = std::move(other.dpi);
		this->rt = std::move(other.rt);

		other.d2dCtx = nullptr;
	}

	return *this;
}

const DirectX::XMFLOAT2 &D2DTargetState::GetDPI() const {
	return this->dpi;
}