#pragma once
#include "DxDeviceCtx.h"
#include "DxDeviceMt.h"

#include <d3d11.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include <dwrite.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <wrl.h>

#include <libhelpers\Thread\PPL\critical_section_guard.h>

struct DxDeviceParams {
	static const uint32_t DefaultD3D11CreateFlags;

	uint32_t d3d11CreateFlags;

	DxDeviceParams();
	DxDeviceParams(uint32_t d3d11CreateFlags);
};

class DxDevice : public DxDeviceMt {
public:
	DxDevice(const DxDeviceParams *params = nullptr);
	~DxDevice();

	// Returns DxDeviceCtx wrapped in critical_section_guard
	// destroy wrapped DxDeviceCtx only when objects from DxDeviceCtx aren't needed.
	critical_section_guard<DxDeviceCtx>::Accessor GetContext();

	D3D_FEATURE_LEVEL GetDeviceFeatureLevel() const;

private:
	// cs-protected
	critical_section_guard<DxDeviceCtx> ctx;

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