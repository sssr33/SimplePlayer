#pragma once
#include "Console\Console.h"
#include "RenderContext.h"

#include <libhelpers\Thread\critical_section.h>
#include <libhelpers\Dx\Renderer\IRenderer.h>
#include <list>

class PlayerRenderer : public IRenderer {
public:
	PlayerRenderer(raw_ptr<DxDevice> dxDev, raw_ptr<IOutput> output);
	virtual ~PlayerRenderer();

	void Render() override;
	void OutputParametersChanged() override;

	void PointerPressed(const DirectX::XMFLOAT2 &pt);
	void PointerMoved(const DirectX::XMFLOAT2 &pt);
	void PointerReleased(const DirectX::XMFLOAT2 &pt);

private:
	thread::critical_section cs;
	RenderContext renderCtx;

	bool inCircle;
	D2D1::Matrix3x2F projection2D, projection2DInv;
	D2D1::Matrix3x2F projection2DLB, projection2DLBInv;
	D2D1::Matrix3x2F projection2DLT, projection2DLTInv;
	D2D1::Matrix3x2F projection2DRB, projection2DRBInv;
	D2D1::Matrix3x2F projection2DRT, projection2DRTInv;

	std::list<D2D1_POINT_2F> points;

	std::list<D2D1_POINT_2F> qbezsBuf;
	std::list<D2D1_BEZIER_SEGMENT> qbezs;

	Console console;

	void DrawMovingLine(ID2D1SolidColorBrush *brush);
};