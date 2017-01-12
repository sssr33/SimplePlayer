#include "MappedResource.h"
#include "libhelpers\HSystem.h"

MappedResource::MappedResource(ID3D11DeviceContext *d3dCtx, ID3D11Resource *res, D3D11_MAP mapType, uint32_t subresource)
	: d3dCtx(d3dCtx), res(res), subresource(subresource)
{
	HRESULT hr = S_OK;

	hr = d3dCtx->Map(res, subresource, mapType, 0, &this->mappedRes);
	H::System::ThrowIfFailed(hr);
}

MappedResource::MappedResource(MappedResource &&other) 
	: d3dCtx(std::move(other.d3dCtx)), res(std::move(other.res)),
	subresource(std::move(other.subresource))
{
	other.d3dCtx = nullptr;
	other.res = nullptr;
}

MappedResource::~MappedResource() {
	if (this->d3dCtx) {
		this->d3dCtx->Unmap(this->res, this->subresource);
	}
}

MappedResource &MappedResource::operator=(MappedResource &&other) {
	if (this != &other) {
		this->d3dCtx = std::move(other.d3dCtx);
		this->res = std::move(other.res);
		this->subresource = std::move(other.subresource);

		other.d3dCtx = nullptr;
		other.res = nullptr;
	}

	return *this;
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