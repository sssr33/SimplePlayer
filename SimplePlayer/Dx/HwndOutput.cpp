#include "HwndOutput.h"

#include <libhelpers\HMath.h>
#include <libhelpers\HSystem.h>

const uint32_t HwndOutput::BufferCount = 2;
const DXGI_FORMAT HwndOutput::BufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;

HwndOutput::HwndOutput(raw_ptr<DxDevice> dxDev, HWND hwnd) 
	: dxDev(dxDev), hwnd(hwnd), logicalDpi(96.0f),
	logicalSize(1.0f, 1.0f), physicalSize(1.0f, 1.0f)
{
	this->d2dOrientationTransform = H::Math::Identity<D2D1_MATRIX_3X2_F>();
	this->d3dOrientationTransform = H::Math::Identity<DirectX::XMFLOAT4X4>();

	this->CreateWindowSizeDependentResources();
}

HwndOutput::~HwndOutput() {
}

float HwndOutput::GetLogicalDpi() const {
	return this->logicalDpi;
}

DirectX::XMFLOAT2 HwndOutput::GetLogicalSize() const {
	return this->logicalSize;
}

D3D11_VIEWPORT HwndOutput::GetD3DViewport() const {
	D3D11_VIEWPORT viewport;

	viewport.TopLeftX = viewport.TopLeftY = 0.0f;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.Width = this->physicalSize.x;
	viewport.Height = this->physicalSize.y;

	return viewport;
}

ID3D11RenderTargetView *HwndOutput::GetD3DRtView() const {
	return this->d3dRenderTargetView.Get();
}

ID2D1Bitmap1 *HwndOutput::GetD2DRtView() const {
	return this->d2dTargetBitmap.Get();
}

D2D1_MATRIX_3X2_F HwndOutput::GetD2DOrientationTransform() const {
	return this->d2dOrientationTransform;
}

DirectX::XMFLOAT4X4 HwndOutput::GetD3DOrientationTransform() const {
	return this->d3dOrientationTransform;
}

void HwndOutput::SetLogicalSize(const DirectX::XMFLOAT2 &v) {
	this->logicalSize = v;
}

void HwndOutput::Resize() {
	this->CreateWindowSizeDependentResources();
}

void HwndOutput::BeginRender() {
	auto rtView = this->GetD3DRtView();
	ID3D11RenderTargetView *const targets[1] = { this->GetD3DRtView() };
	auto ctx = this->dxDev->GetContext();

	ctx->D3D()->OMSetRenderTargets(1, targets, nullptr);

	ctx->D3D()->ClearRenderTargetView(this->GetD3DRtView(), DirectX::Colors::Transparent);
}

void HwndOutput::EndRender() {
	// The first argument instructs DXGI to block until VSync, putting the application
	// to sleep until the next VSync. This ensures we don't waste any cycles rendering
	// frames that will never be displayed to the screen.
	HRESULT hr = this->swapChain->Present(1, 0);

	{
		auto ctx = this->dxDev->GetContext();

		// Discard the contents of the render target.
		// This is a valid operation only when the existing contents will be entirely
		// overwritten. If dirty or scroll rects are used, this call should be modified.
		ctx->D3D()->DiscardView1(this->d3dRenderTargetView.Get(), nullptr, 0);
	}

	//// If the device was removed either by a disconnection or a driver upgrade, we 
	//// must recreate all device resources.
	//if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	//{
	//	HandleDeviceLost();
	//}
	//else
	//{
	//	DX::ThrowIfFailed(hr);
	//}
}

void HwndOutput::CreateWindowSizeDependentResources() {
	this->CleanContextState();

	this->UpdatePresentationParameters();

	if (this->swapChain) {
		HRESULT hr = S_OK;
		auto physSize = this->GetRoundedPhysicalSize();

		hr = this->swapChain->ResizeBuffers(
			HwndOutput::BufferCount, 
			physSize.x, physSize.y, 
			HwndOutput::BufferFormat, 
			0);
		H::System::ThrowIfFailed(hr);
	}
	else {
		this->CreateSwapChain();
	}

	this->CreateTargetViews();
}

void HwndOutput::CleanContextState() {
	ID3D11RenderTargetView* nullViews[] = { nullptr };

	this->d3dRenderTargetView = nullptr;
	this->d2dTargetBitmap = nullptr;

	{
		auto ctx = this->dxDev->GetContext();

		ctx->D2D()->SetTarget(nullptr);
		ctx->D3D()->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
		ctx->D3D()->Flush();
	}
}

void HwndOutput::UpdatePresentationParameters() {
	this->logicalSize.x = (std::max)(this->logicalSize.x, 1.0f);
	this->logicalSize.y = (std::max)(this->logicalSize.y, 1.0f);

	this->physicalSize.x = this->ConvertDipsToPixels(this->logicalSize.x, this->logicalDpi);
	this->physicalSize.y = this->ConvertDipsToPixels(this->logicalSize.y, this->logicalDpi);
}

void HwndOutput::CreateSwapChain() {
	HRESULT hr = S_OK;
	Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDev;
	Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
	Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	auto d3dDev = this->dxDev->GetD3DDeviceCPtr();
	auto physSize = this->GetRoundedPhysicalSize();

	swapChainDesc.BufferDesc.Width = physSize.x;
	swapChainDesc.BufferDesc.Height = physSize.y;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0; // default
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 0; //default
	swapChainDesc.BufferDesc.Format = HwndOutput::BufferFormat;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = HwndOutput::BufferCount;
	swapChainDesc.OutputWindow = this->hwnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	hr = d3dDev.As(&dxgiDev);
	H::System::ThrowIfFailed(hr);

	hr = dxgiDev->GetAdapter(dxgiAdapter.GetAddressOf());
	H::System::ThrowIfFailed(hr);

	hr = dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf()));
	H::System::ThrowIfFailed(hr);

	hr = dxgiFactory->CreateSwapChain(d3dDev.Get(), &swapChainDesc, this->swapChain.ReleaseAndGetAddressOf());
	H::System::ThrowIfFailed(hr);
}

void HwndOutput::CreateTargetViews() {
	HRESULT hr = S_OK;
	auto d3dDev = this->dxDev->GetD3DDevice();
	auto d2dCtxMt = this->dxDev->GetD2DCtxMt();
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	Microsoft::WRL::ComPtr<IDXGISurface> dxgiBackBuffer;
	D2D1_BITMAP_PROPERTIES1 bitmapProperties =
		D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(HwndOutput::BufferFormat, D2D1_ALPHA_MODE_PREMULTIPLIED),
			this->logicalDpi,
			this->logicalDpi
		);

	hr = this->swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));
	H::System::ThrowIfFailed(hr);

	hr = this->swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	H::System::ThrowIfFailed(hr);

	hr = d3dDev->CreateRenderTargetView(
		backBuffer.Get(),
		nullptr,
		this->d3dRenderTargetView.GetAddressOf());
	H::System::ThrowIfFailed(hr);

	hr = d2dCtxMt->CreateBitmapFromDxgiSurface(
		dxgiBackBuffer.Get(),
		bitmapProperties,
		this->d2dTargetBitmap.GetAddressOf());
	H::System::ThrowIfFailed(hr);

	{
		auto viewport = this->GetD3DViewport();
		auto ctx = this->dxDev->GetContext();

		ctx->D3D()->RSSetViewports(1, &viewport);

		ctx->D2D()->SetTarget(this->d2dTargetBitmap.Get());
		ctx->D2D()->SetDpi(this->logicalDpi, this->logicalDpi);
		ctx->D2D()->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
	}
}

DirectX::XMUINT2 HwndOutput::GetRoundedPhysicalSize() const {
	DirectX::XMUINT2 size;

	size.x = (uint32_t)std::ceilf(this->physicalSize.x);
	size.y = (uint32_t)std::ceilf(this->physicalSize.y);

	return size;
}

float HwndOutput::ConvertDipsToPixels(float dips, float dpi) {
	static const float dipsPerInch = 96.0f;
	return floorf(dips * dpi / dipsPerInch + 0.5f); // Round to nearest integer.
}