#pragma once
#include "D2DCtxMt.h"

#include <d3d11.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include <dwrite.h>

// directx interfaces that can be used in multithreaded mode
class DxDeviceMt {
public:
	IDWriteFactory *GetDwriteFactory() const;
	ID2D1Factory1 *GetD2DFactory() const;
	ID3D11Device *GetD3DDevice() const;
	ID2D1Device *GetD2DDevice() const;
	Microsoft::WRL::ComPtr<IDWriteFactory> GetDwriteFactoryCPtr() const;
	Microsoft::WRL::ComPtr<ID2D1Factory1> GetD2DFactoryCPtr() const;
	Microsoft::WRL::ComPtr<ID3D11Device> GetD3DDeviceCPtr() const;
	Microsoft::WRL::ComPtr<ID2D1Device> GetD2DDeviceCPtr() const;
	D2DCtxMt *GetD2DCtxMt() const;

protected:
	Microsoft::WRL::ComPtr<IDWriteFactory> dwriteFactory;
	Microsoft::WRL::ComPtr<ID2D1Factory1> d2dFactory;
	Microsoft::WRL::ComPtr<ID3D11Device> d3dDev;
	Microsoft::WRL::ComPtr<ID2D1Device> d2dDevice;
	D2DCtxMt d2dCtxMt;
};