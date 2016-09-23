#pragma once
#include "DxDeviceCtxProvider.h"

class DxDeviceCtx : public DxDeviceCtxProvider {
public:
	DxDeviceCtx();
	DxDeviceCtx(
		const Microsoft::WRL::ComPtr<ID3D11DeviceContext1> &d3dCtx,
		const Microsoft::WRL::ComPtr<ID2D1DeviceContext> &d2dCtx);
	DxDeviceCtx(const DxDeviceCtx &other);
	DxDeviceCtx(DxDeviceCtx &&other);
	virtual ~DxDeviceCtx();

	DxDeviceCtx &operator=(const DxDeviceCtx &other);
	DxDeviceCtx &operator=(DxDeviceCtx &&other);

	ID3D11DeviceContext1 *D3D() const override;
	ID2D1DeviceContext *D2D() const override;

private:
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1> d3dCtx;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2dCtx;
};