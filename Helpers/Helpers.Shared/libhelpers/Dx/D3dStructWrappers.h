#pragma once
#include "DxIncludes.h"

struct WD3D11_SAMPLER_DESC : public D3D11_SAMPLER_DESC {
	WD3D11_SAMPLER_DESC();
	WD3D11_SAMPLER_DESC(D3D11_FILTER filter);
};