#pragma once
#include "DxDevice.h"

class MappedResource {
public:
	MappedResource(ID3D11DeviceContext *d3dCtx, ID3D11Resource *res, D3D11_MAP mapType, uint32_t subresource = 0);
	~MappedResource();

	void *GetData() const;
	uint32_t GetRowPitch() const;
	uint32_t GetDepthPitch() const;

private:
	ID3D11DeviceContext *d3dCtx;
	ID3D11Resource *res;
	D3D11_MAPPED_SUBRESOURCE mappedRes;
	uint32_t subresource;
};