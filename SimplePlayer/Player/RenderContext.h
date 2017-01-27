#pragma once

#include <libhelpers\MatrixMultiStack.h>
#include <libhelpers\Dx\Dx.h>
#include <libhelpers\Dx\Layers.h>

class RenderContext {
public:
	enum class Matrix {
		Object,
		Camera,
		Projection,
		Orientation,

		Count
	};

	RenderContext(DxDevice *dxDev);

	DxDevice *GetDxDev();
	DxLayerStack *GetLayers();
	MatrixMultiStack *GetMatrixStack();

private:
	DxDevice *dxDev;
	DxLayerStackResources layerRes;
	DxLayerStack layers;

	MatrixMultiStack matrices;
};