#include "D3dStructWrappers.h"

WD3D11_SAMPLER_DESC::WD3D11_SAMPLER_DESC() {
	this->Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	this->AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	this->AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	this->AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	this->MinLOD = -FLT_MAX;
	this->MaxLOD = FLT_MAX;
	this->MipLODBias = 0.0f;
	this->MaxAnisotropy = 1;
	this->ComparisonFunc = D3D11_COMPARISON_NEVER;
	this->BorderColor[0] = this->BorderColor[1] =
		this->BorderColor[2] = this->BorderColor[3] = 0.0f;
}

WD3D11_SAMPLER_DESC::WD3D11_SAMPLER_DESC(D3D11_FILTER filter) {
	this->Filter = filter;
	this->AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	this->AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	this->AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	this->MinLOD = -FLT_MAX;
	this->MaxLOD = FLT_MAX;
	this->MipLODBias = 0.0f;
	this->MaxAnisotropy = 1;
	this->ComparisonFunc = D3D11_COMPARISON_NEVER;
	this->BorderColor[0] = this->BorderColor[1] =
		this->BorderColor[2] = this->BorderColor[3] = 0.0f;
}