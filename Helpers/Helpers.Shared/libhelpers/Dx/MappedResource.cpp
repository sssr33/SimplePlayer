#include "MappedResource.h"
#include "libhelpers\HSystem.h"

MappedResource::MappedResource(ID3D11DeviceContext *d3dCtx, ID3D11Resource *res, D3D11_MAP mapType, uint32_t subresource)
	: d3dCtx(d3dCtx), res(res), subresource(subresource)
{
	HRESULT hr = S_OK;

	hr = d3dCtx->Map(res, subresource, mapType, 0, &this->mappedRes);
	H::System::ThrowIfFailed(hr);
}

MappedResource::~MappedResource() {
	this->d3dCtx->Unmap(this->res, this->subresource);
}

void *MappedResource::GetData() const {
	return this->mappedRes.pData;
}

uint32_t MappedResource::GetRowPitch() const {
	return this->mappedRes.RowPitch;
}

uint32_t MappedResource::GetDepthPitch() const{
	return this->mappedRes.DepthPitch;
}