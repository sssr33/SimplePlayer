#include "DxDevice.h"
#include "libhelpers\HSystem.h"
#include "libhelpers\Macros.h"

const uint32_t DxDeviceParams::DefaultD3D11CreateFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

DxDeviceParams::DxDeviceParams() 
	: d3d11CreateFlags(0)
{
}

DxDeviceParams::DxDeviceParams(uint32_t d3d11CreateFlags) 
	: d3d11CreateFlags(d3d11CreateFlags)
{
}




DxDevice::DxDevice(const DxDeviceParams *params)
	: featureLevel(D3D_FEATURE_LEVEL_9_1)
{
	this->CreateDeviceIndependentResources();
	this->CreateDeviceDependentResources(params);
}

DxDevice::~DxDevice() {
}

critical_section_guard<DxDeviceCtx>::Accessor DxDevice::GetContext() {
	return this->ctx.Get();
}

D3D_FEATURE_LEVEL DxDevice::GetDeviceFeatureLevel() const {
	return this->featureLevel;
}

void DxDevice::CreateDeviceIndependentResources() {
	HRESULT hr = S_OK;

	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		(IUnknown **)this->dwriteFactory.GetAddressOf());
	H::System::ThrowIfFailed(hr);

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, IID_PPV_ARGS(this->d2dFactory.GetAddressOf()));
	H::System::ThrowIfFailed(hr);
}

void DxDevice::CreateDeviceDependentResources(const DxDeviceParams *params) {
	HRESULT hr = S_OK;
	uint32_t flags;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dCtxTmp;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1> d3dCtx;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2dCtx;

	if (params) {
		flags = params->d3d11CreateFlags;
	}
	else {
		flags = DxDeviceParams::DefaultD3D11CreateFlags;
	}

#ifdef _DEBUG
	if (DxDevice::SdkLayersAvailable()) {
		flags |= D3D11_CREATE_DEVICE_DEBUG;
	}
#endif

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, flags,
		featureLevels, ARRAY_SIZE(featureLevels),
		D3D11_SDK_VERSION,
		this->d3dDev.GetAddressOf(), &this->featureLevel,
		d3dCtxTmp.GetAddressOf());

	if (FAILED(hr)) {
		hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_WARP,
			nullptr, flags,
			featureLevels, ARRAY_SIZE(featureLevels),
			D3D11_SDK_VERSION,
			this->d3dDev.GetAddressOf(), &this->featureLevel,
			d3dCtxTmp.GetAddressOf());
		H::System::ThrowIfFailed(hr);
	}

	hr = d3dCtxTmp.As(&d3dCtx);
	H::System::ThrowIfFailed(hr);

	this->EnableD3DDeviceMultithreading();
	this->CreateD2DDevice();
	d2dCtx = this->CreateD2DDeviceContext();

	this->d2dCtxMt = D2DCtxMt(d2dCtx);

	auto ctxAcc = this->ctx.Get();
	*ctxAcc = DxDeviceCtx(d3dCtx, d2dCtx);
}

void DxDevice::EnableD3DDeviceMultithreading() {
	HRESULT hr = S_OK;
	Microsoft::WRL::ComPtr<ID3D10Multithread> multithread;

	hr = this->d3dDev.As(&multithread);
	H::System::ThrowIfFailed(hr);

	multithread->SetMultithreadProtected(TRUE);
}

void DxDevice::CreateD2DDevice() {
	HRESULT hr = S_OK;
	D2D1_CREATION_PROPERTIES creationProps;
	Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDev;

	hr = this->d3dDev.As(&dxgiDev);
	H::System::ThrowIfFailed(hr);

#ifdef _DEBUG
	creationProps.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#else
	creationProps.debugLevel = D2D1_DEBUG_LEVEL_NONE;
#endif

	creationProps.options = D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS;
	creationProps.threadingMode = D2D1_THREADING_MODE_MULTI_THREADED;

	/*
	https://msdn.microsoft.com/query/dev14.query?appId=Dev14IDEF1&l=EN-US&k=k%28d2d1_1%2FD2D1CreateDevice%29;k%28D2D1CreateDevice%29;k%28DevLang-C%2B%2B%29;k%28TargetOS-Windows%29&rd=true
	It's probably better to use D2DFactory::CreateDevice
	to use same d2dFactory
	*/
	hr = this->d2dFactory->CreateDevice(dxgiDev.Get(), this->d2dDevice.GetAddressOf());
	H::System::ThrowIfFailed(hr);

	/*hr = D2D1CreateDevice(dxgiDev.Get(), creationProps, this->d2dDevice.GetAddressOf());
	H::System::ThrowIfFailed(hr);*/
}

Microsoft::WRL::ComPtr<ID2D1DeviceContext> DxDevice::CreateD2DDeviceContext() {
	HRESULT hr = S_OK;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2dCtx;

	hr = this->d2dDevice->CreateDeviceContext(
		D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,
		d2dCtx.GetAddressOf());
	H::System::ThrowIfFailed(hr);

	return d2dCtx;
}

bool DxDevice::SdkLayersAvailable() {
	HRESULT hr = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_NULL,       // There is no need to create a real hardware device.
		0,
		D3D11_CREATE_DEVICE_DEBUG,  // Check for the SDK layers.
		nullptr,                    // Any feature level will do.
		0,
		D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Windows Store apps.
		nullptr,                    // No need to keep the D3D device reference.
		nullptr,                    // No need to know the feature level.
		nullptr                     // No need to keep the D3D device context reference.
	);

	return SUCCEEDED(hr);
}




DxVideoDevice::DxVideoDevice() 
	: DxDevice(
		&DxDeviceParams(
			DxDeviceParams::DefaultD3D11CreateFlags | D3D11_CREATE_DEVICE_VIDEO_SUPPORT))
{
}