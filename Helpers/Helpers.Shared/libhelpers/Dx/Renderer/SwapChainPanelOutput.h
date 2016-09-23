#pragma once
#include "libhelpers\config.h"
#include "IOutput.h"
#include "../Dx.h"
#include "../../raw_ptr.h"

#if HAVE_WINRT == 1
class SwapChainPanelOutput : public IOutput {
	static const uint32_t BufferCount = 2;
	static const DXGI_FORMAT BufferFmt = DXGI_FORMAT_B8G8R8A8_UNORM;
public:
	SwapChainPanelOutput(
		raw_ptr<DxDevice> dxDev,
		Windows::UI::Xaml::Controls::SwapChainPanel ^swapChainPanel);
	virtual ~SwapChainPanelOutput();

	virtual float GetLogicalDpi() const override;
	virtual DirectX::XMFLOAT2 GetLogicalSize() const override;
	virtual D3D11_VIEWPORT GetD3DViewport() const override;
	virtual ID3D11RenderTargetView *GetD3DRtView() const override;
	virtual ID2D1Bitmap1 *GetD2DRtView() const override;
	virtual D2D1_MATRIX_3X2_F GetD2DOrientationTransform() const override;
	virtual DirectX::XMFLOAT4X4 GetD3DOrientationTransform() const override;

	Windows::UI::Xaml::Controls::SwapChainPanel ^GetSwapChainPanel() const;

	void SetLogicalDpi(float v);
	void SetLogicalSize(const DirectX::XMFLOAT2 &v);
	DirectX::XMFLOAT2 GetCompositionScale() const;
	void SetCompositionScale(const DirectX::XMFLOAT2 &v);
	Windows::Graphics::Display::DisplayOrientations GetCurrentOrientation() const;
	void SetCurrentOrientation(Windows::Graphics::Display::DisplayOrientations v);
	void Resize();

	void BeginRender();
	void EndRender();
	void Present();

private:
	raw_ptr<DxDevice> dxDev;
	Windows::UI::Xaml::Controls::SwapChainPanel ^swapChainPanel;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> d3dRenderTargetView;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> d2dTargetBitmap;

	Windows::Graphics::Display::DisplayOrientations nativeOrientation;
	Windows::Graphics::Display::DisplayOrientations currentOrientation;
	D2D1_MATRIX_3X2_F d2dOrientationTransform;
	DirectX::XMFLOAT4X4 d3dOrientationTransform;

	float logicalDpi;
	DirectX::XMFLOAT2 logicalSize;
	DirectX::XMFLOAT2 physicalSize;
	DirectX::XMFLOAT2 compositionScale;

	void CreateWindowSizeDependentResources();
	void CreateSwapChain();

	void UpdatePresentationParameters();

	DXGI_MODE_ROTATION ComputeDisplayRotation();
	void SetRotationMatrices(DXGI_MODE_ROTATION rotation);

	// Converts a length in device-independent pixels (DIPs) to a length in physical pixels.
	static float ConvertDipsToPixels(float dips, float dpi);
};
#endif