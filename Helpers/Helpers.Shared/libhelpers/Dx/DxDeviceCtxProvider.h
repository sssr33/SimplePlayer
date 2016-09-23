#pragma once

#include <d3d11.h>
#include <d3d11_1.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include <dwrite.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <wrl.h>

class DxDeviceCtxProvider {
public:
	DxDeviceCtxProvider();
	virtual ~DxDeviceCtxProvider();

	virtual ID3D11DeviceContext1 *D3D() const = 0;
	virtual ID2D1DeviceContext *D2D() const = 0;
};