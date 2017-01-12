#include "OMBlendState.h"

OMBlendState::OMBlendState(ID3D11DeviceContext *d3dCtx)
	: d3dCtx(d3dCtx)
{
	this->d3dCtx->OMGetBlendState(this->blendState.GetAddressOf(), this->blendFactor, &this->sampleMask);
}

OMBlendState::~OMBlendState() {
	this->d3dCtx->OMSetBlendState(this->blendState.Get(), this->blendFactor, this->sampleMask);
}