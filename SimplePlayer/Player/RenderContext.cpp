#include "RenderContext.h"

RenderContext::RenderContext(DxDevice *dxDev) 
	: dxDev(dxDev), layerRes(this->dxDev->GetD3DDevice()),
	layers(this->dxDev, &this->layerRes)
{
	this->matrices.Resize(Matrix::Count);
}

DxDevice *RenderContext::GetDxDev() {
	return this->dxDev;
}

DxLayerStack *RenderContext::GetLayers() {
	return &this->layers;
}

MatrixMultiStack *RenderContext::GetMatrixStack() {
	return &this->matrices;
}