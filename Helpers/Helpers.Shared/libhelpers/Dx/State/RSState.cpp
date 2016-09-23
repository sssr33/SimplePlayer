#include "RSState.h"

RSState::RSState(ID3D11DeviceContext *d3dCtx)
	: d3dCtx(d3dCtx)
{
	this->d3dCtx->RSGetState(this->rsState.GetAddressOf());
}

RSState::RSState(RSState &&other)
	: d3dCtx(std::move(other.d3dCtx)), rsState(std::move(other.rsState)) 
{
	other.d3dCtx = nullptr;
}

RSState::~RSState() {
	if (this->d3dCtx) {
		this->d3dCtx->RSSetState(this->rsState.Get());
	}
}

RSState &RSState::operator=(RSState &&other) {
	if (this != &other) {
		this->d3dCtx = std::move(other.d3dCtx);
		this->rsState = std::move(other.rsState);

		other.d3dCtx = nullptr;
	}

	return *this;
}