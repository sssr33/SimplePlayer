#pragma once
#include "..\..\DxDevice.h"
#include "libhelpers\Macros.h"

class OMBlendState {
public:
	NO_COPY_MOVE(OMBlendState);

	OMBlendState(ID3D11DeviceContext *d3dCtx);
	~OMBlendState();

private:
	ID3D11DeviceContext *d3dCtx;

	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
	float blendFactor[4];
	uint32_t sampleMask;
};