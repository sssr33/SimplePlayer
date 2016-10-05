#pragma once

#include <libhelpers\raw_ptr.h>
#include <libhelpers\Dx\Renderer\IOutput.h>

class HwndOutput : public IOutput {
	static const uint32_t BufferCount;
	static const DXGI_FORMAT BufferFormat;

public:
	HwndOutput(raw_ptr<DxDevice> dxDev, HWND hwnd);
	virtual ~HwndOutput();

	float GetLogicalDpi() const override;
	DirectX::XMFLOAT2 GetLogicalSize() const override;
	D3D11_VIEWPORT GetD3DViewport() const override;
	ID3D11RenderTargetView *GetD3DRtView() const override;
	ID2D1Bitmap1 *GetD2DRtView() const override;
	D2D1_MATRIX_3X2_F GetD2DOrientationTransform() const override;
	DirectX::XMFLOAT4X4 GetD3DOrientationTransform() const override;

	void SetLogicalSize(const DirectX::XMFLOAT2 &v);

	void Resize();

	void BeginRender();
	void EndRender();

private:
	raw_ptr<DxDevice> dxDev;
	HWND hwnd;

	float logicalDpi;
	DirectX::XMFLOAT2 logicalSize;
	DirectX::XMFLOAT2 physicalSize;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> d3dRenderTargetView;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> d2dTargetBitmap;
	D2D1_MATRIX_3X2_F d2dOrientationTransform;
	DirectX::XMFLOAT4X4 d3dOrientationTransform;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;

	void CreateWindowSizeDependentResources();
	void CleanContextState();
	void UpdatePresentationParameters();
	void CreateSwapChain();
	void CreateTargetViews();

	DirectX::XMUINT2 GetRoundedPhysicalSize() const;

	// Converts a length in device-independent pixels (DIPs) to a length in physical pixels.
	static float ConvertDipsToPixels(float dips, float dpi);
};