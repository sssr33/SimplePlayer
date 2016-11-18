#include "PlayerRenderer.h"

PlayerRenderer::PlayerRenderer(raw_ptr<DxDevice> dxDev, raw_ptr<IOutput> output) 
	: IRenderer(dxDev, output), inCircle(false), 
	projection2D(D2D1::Matrix3x2F()), projection2DInv(D2D1::Matrix3x2F()),
	projection2DLB(D2D1::Matrix3x2F()), projection2DLBInv(D2D1::Matrix3x2F()),
	projection2DLT(D2D1::Matrix3x2F()), projection2DLTInv(D2D1::Matrix3x2F()),
	projection2DRB(D2D1::Matrix3x2F()), projection2DRBInv(D2D1::Matrix3x2F()),
	projection2DRT(D2D1::Matrix3x2F()), projection2DRTInv(D2D1::Matrix3x2F())
{
}

PlayerRenderer::~PlayerRenderer() {
}

void PlayerRenderer::Render() {
	auto ctx = this->dxDev->GetContext();
	auto d2dCtx = ctx->D2D();
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush2;

	thread::critical_section::scoped_lock lk(this->cs);
	auto ellipseCenter = this->projection2D.TransformPoint(D2D1::Point2F());
	auto ellipseRadius = this->projection2D.TransformPoint(D2D1::Point2F(0.0f, -1.0f + 1.0f)).y;

	/*auto pt0 = this->projection2D.TransformPoint(D2D1::Point2F(-1, -1));
	auto pt1 = this->projection2D.TransformPoint(D2D1::Point2F(1, 1));*/

	auto pt0 = this->projection2DRT.TransformPoint(D2D1::Point2F(0, 0));
	auto pt1 = this->projection2DRT.TransformPoint(D2D1::Point2F(1, 1));

	d2dCtx->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue), brush.GetAddressOf());
	d2dCtx->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), brush2.GetAddressOf());

	d2dCtx->BeginDraw();

	d2dCtx->FillEllipse(D2D1::Ellipse(ellipseCenter, ellipseRadius, ellipseRadius), brush.Get());

	d2dCtx->DrawLine(pt0, pt1, brush2.Get());

	d2dCtx->EndDraw();
}

void PlayerRenderer::OutputParametersChanged() {
	auto size = this->output->GetLogicalSize();

	float normW = 2.0f * (size.x / size.y);
	float normH = 2.0f;

	thread::critical_section::scoped_lock lk(this->cs);

	this->projection2D = 
		D2D1::Matrix3x2F::Scale(1.0f / (normW * 0.5f), 1.0f / (normH * 0.5f))
		* D2D1::Matrix3x2F::Scale(size.x / 2.0f, -size.y / 2.0f)
		* D2D1::Matrix3x2F::Translation(size.x / 2.0f, size.y / 2.0f);

	auto fullNormScale = D2D1::Matrix3x2F::Scale(1.0f / normW, 1.0f / normH);

	// left bottom
	{
		this->projection2DLB =
			fullNormScale
			* D2D1::Matrix3x2F::Scale(size.x, -size.y)
			* D2D1::Matrix3x2F::Translation(0.0f, size.y);
	}

	// left top
	{
		this->projection2DLT =
			fullNormScale
			* D2D1::Matrix3x2F::Scale(size.x, size.y)
			* D2D1::Matrix3x2F::Translation(0.0f, 0.0f);
	}

	// right bottom
	{
		this->projection2DRB =
			fullNormScale
			* D2D1::Matrix3x2F::Scale(-size.x, -size.y)
			* D2D1::Matrix3x2F::Translation(size.x, size.y);
	}

	// right top
	{
		this->projection2DRT =
			fullNormScale
			* D2D1::Matrix3x2F::Scale(-size.x, size.y)
			* D2D1::Matrix3x2F::Translation(size.x, 0.0f);
	}

	this->projection2DInv = this->projection2D;
	this->projection2DLBInv = this->projection2DLB;
	this->projection2DLTInv = this->projection2DLT;
	this->projection2DRBInv = this->projection2DRB;
	this->projection2DRTInv = this->projection2DRT;

	this->projection2DInv.Invert();
	this->projection2DLBInv.Invert();
	this->projection2DLTInv.Invert();
	this->projection2DRBInv.Invert();
	this->projection2DRTInv.Invert();
}

void PlayerRenderer::PointerPressed(const DirectX::XMFLOAT2 &pt) {
	thread::critical_section::scoped_lock lk(this->cs);
}

void PlayerRenderer::PointerMoved(const DirectX::XMFLOAT2 &pt) {
	thread::critical_section::scoped_lock lk(this->cs);
	auto ptTmp = this->projection2DInv.TransformPoint(D2D1::Point2F(pt.x, pt.y));

}

void PlayerRenderer::PointerReleased(const DirectX::XMFLOAT2 &pt) {
	thread::critical_section::scoped_lock lk(this->cs);
}