#include "D3D11ImageIOutput.h"

#include <libhelpers\HSystem.h>
#include <libhelpers\HMath.h>

D3D11ImageIOutput::D3D11ImageIOutput(raw_ptr<DxDevice> dxDev)
	: dxDev(dxDev), logicalDpi(96.0f), logicalSize(0, 0),
	d2dOrientationTransform(D2D1::IdentityMatrix()),
	d3dOrientationTransform(H::Math::Identity<DirectX::XMFLOAT4X4>())
{
}

D3D11ImageIOutput::~D3D11ImageIOutput() {
}

float D3D11ImageIOutput::GetLogicalDpi() const {
	return this->logicalDpi;
}

DirectX::XMFLOAT2 D3D11ImageIOutput::GetLogicalSize() const {
	return this->logicalSize;
}

D3D11_VIEWPORT D3D11ImageIOutput::GetD3DViewport() const {
	D3D11_VIEWPORT viewport;

	viewport.TopLeftX = viewport.TopLeftY = 0.0f;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.Width = this->logicalSize.x;
	viewport.Height = this->logicalSize.y;

	return viewport;
}

ID3D11RenderTargetView *D3D11ImageIOutput::GetD3DRtView() const {
	return this->d3dRenderTargetView.Get();
}

ID2D1Bitmap1 *D3D11ImageIOutput::GetD2DRtView() const {
	return this->d2dTargetBitmap.Get();
}

D2D1_MATRIX_3X2_F D3D11ImageIOutput::GetD2DOrientationTransform() const {
	return this->d2dOrientationTransform;
}

DirectX::XMFLOAT4X4 D3D11ImageIOutput::GetD3DOrientationTransform() const {
	return this->d3dOrientationTransform;
}

void D3D11ImageIOutput::ResetRenderTarget(void *surface) {
	HRESULT hr = S_OK;
	Microsoft::WRL::ComPtr<IDXGISurface> dxgiOutputSurface;
	auto outputResource = this->GetOutputResource(surface);
	auto d3dDev = this->dxDev->GetD3DDevice();
	auto d2dCtxMt = this->dxDev->GetD2DCtxMt();

	hr = outputResource.As(&dxgiOutputSurface);
	H::System::ThrowIfFailed(hr);

	D3D11_TEXTURE2D_DESC outputDesc;
	outputResource->GetDesc(&outputDesc);

	this->logicalSize.x = (float)outputDesc.Width;
	this->logicalSize.y = (float)outputDesc.Height;

	D2D1_BITMAP_PROPERTIES1 bitmapProperties =
		D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(outputDesc.Format, D2D1_ALPHA_MODE_PREMULTIPLIED),
			this->logicalDpi,
			this->logicalDpi
		);

	hr = d3dDev->CreateRenderTargetView(
		outputResource.Get(),
		nullptr,
		this->d3dRenderTargetView.ReleaseAndGetAddressOf());
	H::System::ThrowIfFailed(hr);

	hr = d2dCtxMt->CreateBitmapFromDxgiSurface(
		dxgiOutputSurface.Get(),
		bitmapProperties,
		this->d2dTargetBitmap.ReleaseAndGetAddressOf());
	H::System::ThrowIfFailed(hr);

	{
		auto viewport = this->GetD3DViewport();
		auto ctx = this->dxDev->LockCtxScoped();

		this->dxDev->D3D()->RSSetViewports(1, &viewport);

		this->dxDev->D2D()->SetTarget(this->d2dTargetBitmap.Get());
		this->dxDev->D2D()->SetDpi(this->logicalDpi, this->logicalDpi);
		this->dxDev->D2D()->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
	}
}

bool D3D11ImageIOutput::BeginRender() {
	auto rtView = this->GetD3DRtView();
	if (!rtView) {
		return false;
	}

	ID3D11RenderTargetView *const targets[1] = { rtView };
	auto ctx = this->dxDev->LockCtxScoped();

	this->dxDev->D3D()->OMSetRenderTargets(1, targets, nullptr);

	this->dxDev->D3D()->ClearRenderTargetView(rtView, DirectX::Colors::LightGreen);

	return true;
}

void D3D11ImageIOutput::EndRender() {
	{
		auto ctx = this->dxDev->LockCtxScoped();
		auto d3dCtx = this->dxDev->D3D();

		d3dCtx->Flush(); // not renders on D3DImage without flush

		// Discard the contents of the render target.
		// This is a valid operation only when the existing contents will be entirely
		// overwritten. If dirty or scroll rects are used, this call should be modified.
		// d3dCtx->DiscardView1(this->d3dRenderTargetView.Get(), nullptr, 0);

		// TODO check if discard is needed
	}
}

OrientationTypes D3D11ImageIOutput::GetOrientation() const {
	return OrientationTypes();
}

OrientationTypes D3D11ImageIOutput::GetNativeOrientation() const {
	return OrientationTypes();
}

Microsoft::WRL::ComPtr<ID3D11Texture2D> D3D11ImageIOutput::GetOutputResource(void *surface) {
	HRESULT hr = S_OK;
	HANDLE sharedHandle;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> outputResource;
	Microsoft::WRL::ComPtr<IUnknown> tempResource11;
	Microsoft::WRL::ComPtr<IDXGIResource> dxgiResource;
	Microsoft::WRL::ComPtr<IUnknown> pUnk = (IUnknown*)surface;
	auto d3dDev = this->dxDev->GetD3DDevice();

	hr = pUnk.As(&dxgiResource);
	H::System::ThrowIfFailed(hr);

	hr = dxgiResource->GetSharedHandle(&sharedHandle);
	H::System::ThrowIfFailed(hr);

	hr = d3dDev->OpenSharedResource(sharedHandle, IID_PPV_ARGS(tempResource11.GetAddressOf()));
	H::System::ThrowIfFailed(hr);

	hr = tempResource11.As(&outputResource);
	H::System::ThrowIfFailed(hr);

	return outputResource;
}