#include "IRenderer.h"

IRenderer::IRenderer(raw_ptr<DxDevice> dxDev, raw_ptr<IOutput> output)
	: dxDev(dxDev), output(output)
{
}

IRenderer::~IRenderer() {
}