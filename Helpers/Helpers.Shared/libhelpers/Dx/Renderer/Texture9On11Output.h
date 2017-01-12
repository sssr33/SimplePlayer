#pragma once
#include <winapifamily.h>
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)

#include "libhelpers\config.h"
#include "IOutput.h"
#include "ISurface9Access.h"
#include "..\Dx.h"
#include "..\..\raw_ptr.h"
#include "..\..\Thread\condition_variable.h"

#include <d3d9.h>

#pragma comment(lib, "D3d9.lib")
#pragma comment(lib, "User32.lib")

struct TexBuf9 {
	Microsoft::WRL::ComPtr<IDirect3DTexture9> tex;
	Microsoft::WRL::ComPtr<IDirect3DSurface9> surface;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex11;
	HANDLE sharingHandle;
	bool ready;

	NO_COPY(TexBuf9);

	TexBuf9();
	TexBuf9(TexBuf9 &&other);
	~TexBuf9();

	TexBuf9 &operator=(TexBuf9 &&other);
};

class Texture9On11Output : public IOutput {
	static const DXGI_FORMAT BufferFormat;

public:
	Texture9On11Output(raw_ptr<DxDevice> dxDev);
	virtual ~Texture9On11Output();

	float GetLogicalDpi() const override;
	DirectX::XMFLOAT2 GetLogicalSize() const override;
	D3D11_VIEWPORT GetD3DViewport() const override;
	ID3D11RenderTargetView *GetD3DRtView() const override;
	ID2D1Bitmap1 *GetD2DRtView() const override;
	D2D1_MATRIX_3X2_F GetD2DOrientationTransform() const override;
	DirectX::XMFLOAT4X4 GetD3DOrientationTransform() const override;

	OrientationTypes GetOrientation() const override;
	OrientationTypes GetNativeOrientation() const override;

	void SetLogicalSize(const DirectX::XMFLOAT2 &v);

	DirectX::XMFLOAT4 GetRTColor() const;
	void SetRTColor(const DirectX::XMFLOAT4 &color);

	void Resize();

	void BeginRender();
	void EndRender();
	void WaitForComposition();

	Surface9Data NextD3D9Surface();
	void NextComposition();

private:
	raw_ptr<DxDevice> dxDev;

	float logicalDpi;
	DirectX::XMFLOAT2 logicalSize;
	DirectX::XMFLOAT2 physicalSize;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> d3dRenderTargetView;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> d2dTargetBitmap;
	D2D1_MATRIX_3X2_F d2dOrientationTransform;
	DirectX::XMFLOAT4X4 d3dOrientationTransform;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> renderTarget;

	Microsoft::WRL::ComPtr<IDirect3D9Ex> d3d9;
	Microsoft::WRL::ComPtr<IDirect3DDevice9Ex> d3d9Dev;
	HWND dummyHWND;
	thread::critical_section texBufs9Cs;
	TexBuf9 texBufs9[2];

	bool compositionDone;
	thread::critical_section compositionCs;
	thread::condition_variable compositionCv;

	DirectX::XMFLOAT4 rtColor;

	void CreateWindowSizeDependentResources();
	void CleanContextState();
	void UpdatePresentationParameters();
	void CreateSwapChain();
	void CreateTargetViews();

	DirectX::XMUINT2 GetRoundedPhysicalSize() const;

	// Converts a length in device-independent pixels (DIPs) to a length in physical pixels.
	static float ConvertDipsToPixels(float dips, float dpi);

	static D3DFORMAT DXGIToCrossAPID3D9Format(DXGI_FORMAT Format);
};
#endif