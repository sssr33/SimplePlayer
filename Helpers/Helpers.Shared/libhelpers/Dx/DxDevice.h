#pragma once
#include "DxDeviceMt.h"
#include "DxDeviceCtx.h"

#include <d3d11.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include <dwrite.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <wrl.h>

struct DxDeviceParams {
	static const uint32_t DefaultD3D11CreateFlags;

	uint32_t d3d11CreateFlags;

	DxDeviceParams();
	DxDeviceParams(uint32_t d3d11CreateFlags);
};

class DxDevice : public DxDeviceMt, public DxDeviceCtx {
public:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/dd756649%28v=vs.85%29.aspx
	static const float D2DDefaultDPI;

	DxDevice(const DxDeviceParams *params = nullptr);
	~DxDevice();

	D3D_FEATURE_LEVEL GetDeviceFeatureLevel() const;

	void Trim();

private:
	D3D_FEATURE_LEVEL featureLevel;

	void CreateDeviceIndependentResources();
	void CreateDeviceDependentResources(const DxDeviceParams *params);
	void EnableD3DDeviceMultithreading();
	void CreateD2DDevice();
	Microsoft::WRL::ComPtr<ID2D1DeviceContext> CreateD2DDeviceContext();

	// Check for SDK Layer support.
	static bool SdkLayersAvailable();
};

class DxVideoDevice : public DxDevice {
public:
	DxVideoDevice();
};