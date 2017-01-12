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
#include <libhelpers\Thread\critical_section.h>

/*
Class with protected d3d and d2d context.
Before calling getter or setter LockXXX must be called by caller.
*/
class DxDeviceCtx {
public:
	void LockCtx();
	thread::critical_section::scoped_lock LockCtxScoped();
	thread::critical_section::scoped_yield_lock LockYieldCtxScoped();

	ID3D11DeviceContext1 *D3D() const;
	ID2D1DeviceContext *D2D() const;

	void UnlockCtx();

protected:
	void D3D(const Microsoft::WRL::ComPtr<ID3D11DeviceContext1> &d3dCtx);
	void D2D(const Microsoft::WRL::ComPtr<ID2D1DeviceContext> &d2dCtx);

private:
	// cs-protected d3d and d2d context
	thread::critical_section ctxCs;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1> d3dCtx;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2dCtx;
};