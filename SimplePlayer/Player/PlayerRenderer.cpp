#include "PlayerRenderer.h"

PlayerRenderer::PlayerRenderer(raw_ptr<DxDevice> dxDev, raw_ptr<IOutput> output) 
	: IRenderer(dxDev, output)
{
}

PlayerRenderer::~PlayerRenderer() {
}

void PlayerRenderer::Render() {
	auto ctx = this->dxDev->GetContext();
	auto d2dCtx = ctx->D2D();
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
	auto size = this->output->GetLogicalSize();

	auto transform =
		D2D1::Matrix3x2F::Scale(size.x / 2.0f, -size.y / 2.0f)
		* D2D1::Matrix3x2F::Translation(size.x / 2.0f, size.y / 2.0f);

	auto ellipseCenter = transform.TransformPoint(D2D1::Point2F());
	auto ellipseRadius = transform.TransformPoint(D2D1::Point2F(0.0f, -1.0f + 1.0f)).y;

	d2dCtx->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue), brush.GetAddressOf());

	d2dCtx->BeginDraw();

	d2dCtx->FillEllipse(D2D1::Ellipse(ellipseCenter, ellipseRadius, ellipseRadius), brush.Get());

	d2dCtx->EndDraw();
}

void PlayerRenderer::OutputParametersChanged() {
}