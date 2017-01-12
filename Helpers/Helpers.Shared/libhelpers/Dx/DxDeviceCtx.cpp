#include "DxDeviceCtx.h"

#include <libhelpers\HSystem.h>

void DxDeviceCtx::LockCtx() {
	this->ctxCs.lock();
}

thread::critical_section::scoped_lock DxDeviceCtx::LockCtxScoped() {
	thread::critical_section::scoped_lock lk(this->ctxCs);
	return lk;
}

thread::critical_section::scoped_yield_lock DxDeviceCtx::LockYieldCtxScoped() {
	thread::critical_section::scoped_yield_lock lk(this->ctxCs);
	return lk;
}

ID3D11DeviceContext1 *DxDeviceCtx::D3D() const {
	hAssert(this->ctxCs.owned());
	return this->d3dCtx.Get();
}

ID2D1DeviceContext *DxDeviceCtx::D2D() const {
	hAssert(this->ctxCs.owned());
	return this->d2dCtx.Get();
}

void DxDeviceCtx::UnlockCtx() {
	this->ctxCs.unlock();
}

void DxDeviceCtx::D3D(const Microsoft::WRL::ComPtr<ID3D11DeviceContext1> &d3dCtx) {
	hAssert(this->ctxCs.owned());
	this->d3dCtx = d3dCtx;
}

void DxDeviceCtx::D2D(const Microsoft::WRL::ComPtr<ID2D1DeviceContext> &d2dCtx) {
	hAssert(this->ctxCs.owned());
	this->d2dCtx = d2dCtx;
}