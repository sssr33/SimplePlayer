#include "DxDeviceCtx.h"

#include <utility>

DxDeviceCtx::DxDeviceCtx() {
}

DxDeviceCtx::DxDeviceCtx(
	const Microsoft::WRL::ComPtr<ID3D11DeviceContext1> &d3dCtx,
	const Microsoft::WRL::ComPtr<ID2D1DeviceContext> &d2dCtx)
	: d3dCtx(d3dCtx), d2dCtx(d2dCtx)
{
}

DxDeviceCtx::DxDeviceCtx(const DxDeviceCtx &other)
	: d3dCtx(other.d3dCtx), d2dCtx(other.d2dCtx) {
}

DxDeviceCtx::DxDeviceCtx(DxDeviceCtx &&other)
	: d3dCtx(std::move(other.d3dCtx)), d2dCtx(std::move(other.d2dCtx))
{
}

DxDeviceCtx &DxDeviceCtx::operator=(const DxDeviceCtx &other) {
	if (this != &other) {
		this->d3dCtx = other.d3dCtx;
		this->d2dCtx = other.d2dCtx;
	}

	return *this;
}

DxDeviceCtx &DxDeviceCtx::operator=(DxDeviceCtx &&other) {
	if (this != &other) {
		this->d3dCtx = std::move(other.d3dCtx);
		this->d2dCtx = std::move(other.d2dCtx);
	}

	return *this;
}

DxDeviceCtx::~DxDeviceCtx() {
}

ID3D11DeviceContext1 *DxDeviceCtx::D3D() const {
	return this->d3dCtx.Get();
}

ID2D1DeviceContext *DxDeviceCtx::D2D() const {
	return this->d2dCtx.Get();
}