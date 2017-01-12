#include "DxDeviceMt.h"

IDWriteFactory *DxDeviceMt::GetDwriteFactory() const {
	return this->dwriteFactory.Get();
}

ID2D1Factory1 *DxDeviceMt::GetD2DFactory() const {
	return this->d2dFactory.Get();
}

ID3D11Device *DxDeviceMt::GetD3DDevice() const {
	return this->d3dDev.Get();
}

ID2D1Device *DxDeviceMt::GetD2DDevice() const {
	return this->d2dDevice.Get();
}

D2DCtxMt *DxDeviceMt::GetD2DCtxMt() const {
	// TODO try to find better approach than const_cast
	D2DCtxMt *tmp = const_cast<D2DCtxMt *>(&this->d2dCtxMt);
	return tmp;
}

Microsoft::WRL::ComPtr<IDWriteFactory> DxDeviceMt::GetDwriteFactoryCPtr() const {
	return this->dwriteFactory;
}

Microsoft::WRL::ComPtr<ID2D1Factory1> DxDeviceMt::GetD2DFactoryCPtr() const {
	return this->d2dFactory;
}

Microsoft::WRL::ComPtr<ID3D11Device> DxDeviceMt::GetD3DDeviceCPtr() const {
	return this->d3dDev;
}

Microsoft::WRL::ComPtr<ID2D1Device> DxDeviceMt::GetD2DDeviceCPtr() const {
	return this->d2dDevice;
}