#include "Texture9On11Output.h"

#include <winapifamily.h>
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)

#include "..\..\HMath.h"
#include "..\..\HSystem.h"
#include "..\..\HColor.h"

TexBuf9::TexBuf9()
	: sharingHandle(nullptr), ready(false)
{
}

TexBuf9::TexBuf9(TexBuf9 &&other)
	: tex(std::move(other.tex)),
	surface(std::move(other.surface)),
	tex11(std::move(other.tex11))
{
	this->sharingHandle = other.sharingHandle;
	this->ready = other.ready;

	other.sharingHandle = nullptr;
	other.ready = false;
}

TexBuf9::~TexBuf9() {
	/*if (this->sharingHandle != nullptr) {
	BOOL res = CloseHandle(this->sharingHandle);
	}*/
}

TexBuf9 &TexBuf9::operator=(TexBuf9 &&other) {
	if (this != &other) {
		/*if (this->sharingHandle != nullptr) {
		BOOL res = CloseHandle(this->sharingHandle);
		}*/

		this->tex = std::move(other.tex);
		this->surface = std::move(other.surface);
		this->tex11 = std::move(other.tex11);

		this->sharingHandle = other.sharingHandle;
		this->ready = other.ready;

		other.sharingHandle = nullptr;
		other.ready = false;
	}

	return *this;
}




const DXGI_FORMAT Texture9On11Output::BufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;

Texture9On11Output::Texture9On11Output(raw_ptr<DxDevice> dxDev)
	: dxDev(dxDev), logicalDpi(96.0f),
	logicalSize(1.0f, 1.0f), physicalSize(1.0f, 1.0f), compositionDone(false)
{
	HRESULT hr = S_OK;
	auto dxLk = this->dxDev->LockCtxScoped();

	{
		auto tmp = DirectX::Colors::Transparent;
		this->rtColor = { tmp.f[0], tmp.f[1], tmp.f[2], tmp.f[3] };
	}

	this->d2dOrientationTransform = H::Math::Identity<D2D1_MATRIX_3X2_F>();
	this->d3dOrientationTransform = H::Math::Identity<DirectX::XMFLOAT4X4>();

	hr = Direct3DCreate9Ex(D3D_SDK_VERSION, this->d3d9.GetAddressOf());
	H::System::ThrowIfFailed(hr);

	D3DPRESENT_PARAMETERS d3dpp = {};

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = NULL;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	this->dummyHWND = CreateWindowA("STATIC", "dummy", NULL, 0, 0, 100, 100, NULL, NULL, NULL, NULL);

	hr = this->d3d9->CreateDeviceEx(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		this->dummyHWND,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED | D3DCREATE_FPU_PRESERVE,
		&d3dpp,
		NULL,
		this->d3d9Dev.GetAddressOf());
	H::System::ThrowIfFailed(hr);

	this->CreateWindowSizeDependentResources();
}

Texture9On11Output::~Texture9On11Output() {
	DestroyWindow(this->dummyHWND);
}

float Texture9On11Output::GetLogicalDpi() const {
	return this->logicalDpi;
}

DirectX::XMFLOAT2 Texture9On11Output::GetLogicalSize() const {
	return this->logicalSize;
}

D3D11_VIEWPORT Texture9On11Output::GetD3DViewport() const {
	D3D11_VIEWPORT viewport;

	viewport.TopLeftX = viewport.TopLeftY = 0.0f;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.Width = this->physicalSize.x;
	viewport.Height = this->physicalSize.y;

	return viewport;
}

ID3D11RenderTargetView *Texture9On11Output::GetD3DRtView() const {
	return this->d3dRenderTargetView.Get();
}

ID2D1Bitmap1 *Texture9On11Output::GetD2DRtView() const {
	return this->d2dTargetBitmap.Get();
}

D2D1_MATRIX_3X2_F Texture9On11Output::GetD2DOrientationTransform() const {
	return this->d2dOrientationTransform;
}

DirectX::XMFLOAT4X4 Texture9On11Output::GetD3DOrientationTransform() const {
	return this->d3dOrientationTransform;
}

OrientationTypes Texture9On11Output::GetOrientation() const {
	return OrientationTypes::None;
}

OrientationTypes Texture9On11Output::GetNativeOrientation() const {
	return OrientationTypes::None;
}

void Texture9On11Output::SetLogicalSize(const DirectX::XMFLOAT2 &v) {
	this->logicalSize = v;
}

DirectX::XMFLOAT4 Texture9On11Output::GetRTColor() const {
	return this->rtColor;
}

void Texture9On11Output::SetRTColor(const DirectX::XMFLOAT4 &color) {
	this->rtColor = color;
}

void Texture9On11Output::Resize() {
	this->CreateWindowSizeDependentResources();
}

void Texture9On11Output::BeginRender() {
	auto rtView = this->GetD3DRtView();
	ID3D11RenderTargetView *const targets[1] = { this->GetD3DRtView() };

	auto premultiplied = H::Color::PremultiplyColor(this->rtColor);
	DirectX::XMVECTORF32 tmp = {
		premultiplied.x,
		premultiplied.y,
		premultiplied.z,
		premultiplied.w };

	this->dxDev->D3D()->OMSetRenderTargets(1, targets, nullptr);
	this->dxDev->D3D()->ClearRenderTargetView(this->GetD3DRtView(), tmp);
}

void Texture9On11Output::EndRender() {
	{
		auto d3dCtx = this->dxDev->D3D();

		//d3dCtx->Flush();

		{
			// d3d surface
			thread::critical_section::scoped_lock lk(this->texBufs9Cs);
			bool resize = true;
			D3D11_TEXTURE2D_DESC srcDesc;
			auto d3dDev = this->dxDev->GetD3DDevice();

			this->renderTarget->GetDesc(&srcDesc);

			if (texBufs9[0].tex) {
				D3DSURFACE_DESC dstDesc;

				texBufs9[0].tex->GetLevelDesc(0, &dstDesc);

				resize = srcDesc.Width != dstDesc.Width || srcDesc.Height != dstDesc.Height;
			}
			HRESULT hr = S_OK;
			if (resize) {
				texBufs9[0] = TexBuf9();

				hr = this->d3d9Dev->CreateTexture(
					srcDesc.Width, srcDesc.Height,
					1,
					D3DUSAGE_RENDERTARGET,
					Texture9On11Output::DXGIToCrossAPID3D9Format(srcDesc.Format),
					D3DPOOL_DEFAULT,
					texBufs9[0].tex.GetAddressOf(),
					&texBufs9[0].sharingHandle);
				H::System::ThrowIfFailed(hr);

				hr = texBufs9[0].tex->GetSurfaceLevel(0, texBufs9[0].surface.GetAddressOf());
				H::System::ThrowIfFailed(hr);

				hr = d3dDev->OpenSharedResource(texBufs9[0].sharingHandle, IID_PPV_ARGS(texBufs9[0].tex11.GetAddressOf()));
				H::System::ThrowIfFailed(hr);
			}

			/*Microsoft::WRL::ComPtr<ID3D11Texture2D> dst;

			hr = d3dDev->OpenSharedResource(texBufs9[0].sharingHandle, IID_PPV_ARGS(dst.GetAddressOf()));
			H::System::ThrowIfFailed(hr);*/

			d3dCtx->CopyResource(texBufs9[0].tex11.Get(), this->renderTarget.Get());
			d3dCtx->Flush();

			if (texBufs9[0].ready) {
				int stop = 432;
			}

			texBufs9[0].ready = true;
		}

		// Discard the contents of the render target.
		// This is a valid operation only when the existing contents will be entirely
		// overwritten. If dirty or scroll rects are used, this call should be modified.
		//ctx->D3D()->DiscardView1(this->d3dRenderTargetView.Get(), nullptr, 0);
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

void Texture9On11Output::WaitForComposition() {
	thread::critical_section::scoped_lock lk(this->compositionCs);

	while (!this->compositionDone) {
		this->compositionCv.wait(this->compositionCs);
	}

	this->compositionDone = false;
}

Surface9Data Texture9On11Output::NextD3D9Surface() {
	Surface9Data data;

	//{
	//	thread::critical_section::scoped_lock lk(this->texBufs9Cs);

	//	std::swap(texBufs9[0], texBufs9[1]);

	//	/*if (texBufs9[0].tex && texBufs9[0].frameNum > texBufs9[1].frameNum) {
	//		texBufs9[1].frameNum = texBufs9[0].frameNum;
	//		std::swap(texBufs9[0], texBufs9[1]);
	//	}
	//	else {
	//		int stop = 243;
	//	}*/
	//}

	{
		thread::critical_section::scoped_lock lk(this->texBufs9Cs);

		if (texBufs9[0].ready) {
			std::swap(texBufs9[0], texBufs9[1]);
			texBufs9[0].ready = false;

			// start render next frame after current frame taken by user code
			this->NextComposition();
		}
	}

	if (texBufs9[1].tex) {
		HRESULT hr = S_OK;
		D3DSURFACE_DESC desc;

		hr = texBufs9[1].tex->GetLevelDesc(0, &desc);
		H::System::ThrowIfFailed(hr);

		data.surface = texBufs9[1].surface.Get();
		data.width = (int)desc.Width;
		data.height = (int)desc.Height;
	}
	else {
		data.surface = nullptr;
		data.width = 0;
		data.height = 0;
	}

	return data;
}

void Texture9On11Output::NextComposition() {
	{
		thread::critical_section::scoped_lock lk(this->compositionCs);
		this->compositionDone = true;
	}

	this->compositionCv.notify();
}

void Texture9On11Output::CreateWindowSizeDependentResources() {
	this->CleanContextState();

	this->UpdatePresentationParameters();

	this->CreateSwapChain();

	this->CreateTargetViews();
}

void Texture9On11Output::CleanContextState() {
	ID3D11RenderTargetView* nullViews[] = { nullptr };

	this->d3dRenderTargetView = nullptr;
	this->d2dTargetBitmap = nullptr;

	{
		this->dxDev->D2D()->SetTarget(nullptr);
		this->dxDev->D3D()->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
		this->dxDev->D3D()->Flush();
	}
}

void Texture9On11Output::UpdatePresentationParameters() {
	this->logicalSize.x = (std::max)(this->logicalSize.x, 1.0f);
	this->logicalSize.y = (std::max)(this->logicalSize.y, 1.0f);

	this->physicalSize.x = this->ConvertDipsToPixels(this->logicalSize.x, this->logicalDpi);
	this->physicalSize.y = this->ConvertDipsToPixels(this->logicalSize.y, this->logicalDpi);
}

void Texture9On11Output::CreateSwapChain() {
	HRESULT hr = S_OK;
	D3D11_TEXTURE2D_DESC rtDesc;
	auto d3dDev = this->dxDev->GetD3DDeviceCPtr();
	auto physSize = this->GetRoundedPhysicalSize();

	rtDesc.Width = physSize.x;
	rtDesc.Height = physSize.y;
	rtDesc.MipLevels = 1;
	rtDesc.ArraySize = 1;
	rtDesc.Format = Texture9On11Output::BufferFormat;
	rtDesc.SampleDesc.Count = 1;
	rtDesc.SampleDesc.Quality = 0;
	rtDesc.Usage = D3D11_USAGE_DEFAULT;
	rtDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
	rtDesc.CPUAccessFlags = 0;
	rtDesc.MiscFlags = 0;

	hr = d3dDev->CreateTexture2D(&rtDesc, nullptr, this->renderTarget.ReleaseAndGetAddressOf());
	H::System::ThrowIfFailed(hr);
}

void Texture9On11Output::CreateTargetViews() {
	HRESULT hr = S_OK;
	auto d3dDev = this->dxDev->GetD3DDevice();
	auto d2dCtxMt = this->dxDev->GetD2DCtxMt();
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	Microsoft::WRL::ComPtr<IDXGISurface> dxgiBackBuffer;
	D2D1_BITMAP_PROPERTIES1 bitmapProperties =
		D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(Texture9On11Output::BufferFormat, D2D1_ALPHA_MODE_PREMULTIPLIED),
			this->logicalDpi,
			this->logicalDpi
		);

	backBuffer = this->renderTarget;

	hr = this->renderTarget.As(&dxgiBackBuffer);
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

		this->dxDev->D3D()->RSSetViewports(1, &viewport);

		this->dxDev->D2D()->SetTarget(this->d2dTargetBitmap.Get());
		this->dxDev->D2D()->SetDpi(this->logicalDpi, this->logicalDpi);
		this->dxDev->D2D()->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
	}
}

DirectX::XMUINT2 Texture9On11Output::GetRoundedPhysicalSize() const {
	DirectX::XMUINT2 size;

	size.x = (uint32_t)std::ceilf(this->physicalSize.x);
	size.y = (uint32_t)std::ceilf(this->physicalSize.y);

	return size;
}

float Texture9On11Output::ConvertDipsToPixels(float dips, float dpi) {
	static const float dipsPerInch = 96.0f;
	return floorf(dips * dpi / dipsPerInch + 0.5f); // Round to nearest integer.
}

D3DFORMAT Texture9On11Output::DXGIToCrossAPID3D9Format(DXGI_FORMAT Format) {
	switch (Format)
	{
	case DXGI_FORMAT_B8G8R8A8_UNORM:
		return D3DFMT_A8R8G8B8;
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
		return D3DFMT_A8R8G8B8;
	case DXGI_FORMAT_B8G8R8X8_UNORM:
		return D3DFMT_X8R8G8B8;
	case DXGI_FORMAT_R8G8B8A8_UNORM:
		return D3DFMT_A8B8G8R8;
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
		return D3DFMT_A8B8G8R8;
	case DXGI_FORMAT_R10G10B10A2_UNORM:
		return D3DFMT_A2B10G10R10;
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
		return D3DFMT_A16B16G16R16F;
	default:
		return D3DFMT_UNKNOWN;
	};
}

#endif