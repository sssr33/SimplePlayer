#pragma once
#include "DxDevice.h"
#include "..\Macros.h"

class MappedResource {
public:
	NO_COPY(MappedResource);

	MappedResource(ID3D11DeviceContext *d3dCtx, ID3D11Resource *res, D3D11_MAP mapType, uint32_t subresource = 0);
	MappedResource(MappedResource &&other);
	~MappedResource();

	MappedResource &operator=(MappedResource &&other);

	void *GetData() const;
	uint32_t GetRowPitch() const;
	uint32_t GetDepthPitch() const;

private:
	ID3D11DeviceContext *d3dCtx;
	ID3D11Resource *res;
	D3D11_MAPPED_SUBRESOURCE mappedRes;
	uint32_t subresource;
};