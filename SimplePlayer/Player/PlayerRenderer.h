#pragma once

#include <libhelpers\Dx\Renderer\IRenderer.h>

class PlayerRenderer : public IRenderer {
public:
	PlayerRenderer(raw_ptr<DxDevice> dxDev, raw_ptr<IOutput> output);
	virtual ~PlayerRenderer();

	void Render() override;
	void OutputParametersChanged() override;

	void PointerPressed(const DirectX::XMFLOAT2 &pt);
	void PointerMoved(const DirectX::XMFLOAT2 &pt);
	void PointerReleased(const DirectX::XMFLOAT2 &pt);
};