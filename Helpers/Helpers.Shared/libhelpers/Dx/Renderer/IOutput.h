#pragma once
#include "../Dx.h"

class IOutput {
public:
	IOutput();
	virtual ~IOutput();

	virtual float GetLogicalDpi() const = 0;
	virtual DirectX::XMFLOAT2 GetLogicalSize() const = 0;
	virtual D3D11_VIEWPORT GetD3DViewport() const = 0;
	virtual ID3D11RenderTargetView *GetD3DRtView() const = 0;
	virtual ID2D1Bitmap1 *GetD2DRtView() const = 0;
	virtual D2D1_MATRIX_3X2_F GetD2DOrientationTransform() const = 0;
	virtual DirectX::XMFLOAT4X4 GetD3DOrientationTransform() const = 0;

	virtual OrientationTypes GetOrientation() const = 0;
	virtual OrientationTypes GetNativeOrientation() const = 0;
};