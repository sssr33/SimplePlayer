#pragma once
#include "DxLayerStackResources.h"
#include "DxLayerStackState.h"
#include "..\DxDeviceCtxProvider.h"
#include "libhelpers\raw_ptr.h"
#include "libhelpers\ScopedPushBase.h"

#include <d3d11_1.h>
#include <d2d1_1.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>

class DxLayerStack {
public:
	struct PushScope0 : public ScopedPushBase<DxLayerStack> {
		~PushScope0();
	};
	typedef ScopedPushWrapper<DxLayerStack, PushScope0> RenderTargetScope;

	struct PushScope1 : public ScopedPushBase<DxLayerStack> {
		~PushScope1();
	};
	typedef ScopedPushWrapper<DxLayerStack, PushScope1> LayerScope;

	struct PushScope2 : public ScopedPushBase<DxLayerStack> {
		~PushScope2();
	};
	typedef ScopedPushWrapper<DxLayerStack, PushScope2> AxisAlignedClipScope;

	DxLayerStack(
		DxDeviceCtxProvider *dxCtxProv,
		DxLayerStackResources *resources);

	// call before d3d draw calls to apply previous d2d layers to d3d rendering
	DxLayerStackState BeginD3D();

	// call when start rendering to new D3D or/and D2D render target
	RenderTargetScope PushRenderTargetScoped();
	void PushRenderTarget();
	void PopRenderTarget();

	LayerScope PushLayerScoped(const D2D1_LAYER_PARAMETERS &params, ID2D1Layer *layer = nullptr);
	void PushLayer(const D2D1_LAYER_PARAMETERS &params, ID2D1Layer *layer = nullptr);
	void PopLayer();

	AxisAlignedClipScope PushAxisAlignedClipScoped(const D2D1_RECT_F &rect, D2D1_ANTIALIAS_MODE antialiasMode);
	void PushAxisAlignedClip(const D2D1_RECT_F &rect, D2D1_ANTIALIAS_MODE antialiasMode);
	void PopAxisAlignedClip();

private:
	DxDeviceCtxProvider *dxCtxProv;
	DxLayerStackResources *resources;

	std::vector<D2D1_RECT_F> layerSizes;
	std::vector<std::vector<D2D1_RECT_F>> layerSizesStack;

	D2D1_RECT_F GetCurrentRect();

	static D2D1_RECT_F ConcatRects(
		D2D1_RECT_F existing,
		D2D1_RECT_F other,
		D2D1_ANTIALIAS_MODE antialiasMode,
		const D2D1_MATRIX_3X2_F &transform);
	static D2D1_RECT_F ConcatRects(
		const D2D1_RECT_F &existing, 
		const D2D1_RECT_F &other, 
		D2D1_ANTIALIAS_MODE antialiasMode);
	static bool IsRectInfinite(const D2D1_RECT_F &v);
};