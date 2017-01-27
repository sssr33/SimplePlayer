#include "Console.h"

#include <libhelpers\HMath.h>
#include <libhelpers\Dx\State\D2DTransformState.h>

Console::Console(DxDevice *dxDev) 
	: dxDev(dxDev)
{
}

Console::~Console() {
}

void Console::Render(RenderContext &renderCtx) {
	auto d2dCtx = renderCtx.GetDxDev()->D2D();
	D2DTransformState transformState(d2dCtx);

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;

	d2dCtx->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::GreenYellow), brush.GetAddressOf());

	auto &d2dMatrix = renderCtx.GetMatrixStack()->GetD2DMatrix();
	auto rect = H::Math::TransformRect(D2D1::RectF(-0.5f, 0.5f, 0.5f, -0.5f), d2dMatrix);

	d2dCtx->FillRectangle(rect, brush.Get());
}