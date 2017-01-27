#include "PlayerRenderer.h"

#include <vector>
#include <DirectXMath.h>
#include <libhelpers\HMath.h>

PlayerRenderer::PlayerRenderer(raw_ptr<DxDevice> dxDev, raw_ptr<IOutput> output)
	: IRenderer(dxDev, output), renderCtx(this->dxDev.get()), inCircle(false),
	projection2D(D2D1::Matrix3x2F()), projection2DInv(D2D1::Matrix3x2F()),
	projection2DLB(D2D1::Matrix3x2F()), projection2DLBInv(D2D1::Matrix3x2F()),
	projection2DLT(D2D1::Matrix3x2F()), projection2DLTInv(D2D1::Matrix3x2F()),
	projection2DRB(D2D1::Matrix3x2F()), projection2DRBInv(D2D1::Matrix3x2F()),
	projection2DRT(D2D1::Matrix3x2F()), projection2DRTInv(D2D1::Matrix3x2F()),
	console(this->dxDev.get())
{
}

PlayerRenderer::~PlayerRenderer() {
}

void PlayerRenderer::Render() {
	auto d2dFactory = this->dxDev->GetD2DFactory();
	//auto ctx = this->dxDev->GetContext();
	auto dxLk = this->dxDev->LockCtxScoped();
	auto d2dCtx = this->dxDev->D2D();
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

	auto projMatrixScope = this->renderCtx.GetMatrixStack()->PushScoped(RenderContext::Matrix::Projection, this->projection2D);

	this->console.Render(this->renderCtx);

	this->DrawMovingLine(brush2.Get());

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

	//auto pt2 = D2D1::Point2F(pt.x, pt.y);

	//if (qbezsBuf.size() < 2) {
	//	qbezsBuf.push_back(pt2);
	//}
	//else {
	//	auto pt1It = qbezsBuf.end();
	//	--pt1It;
	//	auto pt0It = pt1It;
	//	--pt0It;

	//	/*auto tmp0 = DirectX::XMVectorSet(pt0It->x, pt0It->y, 0.0f, 1.0f);
	//	auto tmp1 = DirectX::XMVectorSet(pt1It->x, pt1It->y, 0.0f, 1.0f);*/

	//	auto tmp0 = DirectX::XMVectorSet(pt1It->x, pt1It->y, 0.0f, 1.0f);
	//	auto tmp1 = DirectX::XMVectorSet(pt2.x, pt2.y, 0.0f, 1.0f);

	//	auto vec = DirectX::XMVectorSubtract(tmp1, tmp0);
	//	auto len = DirectX::XMVector2Length(vec).XF;
	//	/*qbezsBuf.push_back(pt2);*/
	//	if (len > 100) {
	//		qbezsBuf.push_back(pt2);

	//		if (len > 50) {
	//			int stop = 23;
	//		}
	//	}
	//	/*else {
	//		pt1It->x = (pt1It->x + pt2.x) * 0.5f;
	//		pt1It->y = (pt1It->y + pt2.y) * 0.5f;
	//	}*/
	//}

	//if (qbezsBuf.size() > 3) {
	//	auto it = qbezsBuf.begin();
	//	auto it2 = it;
	//	auto it3 = it;
	//	++it2;
	//	++it3;
	//	++it3;
	//	auto seg = D2D1::BezierSegment(*it, *it2, *it3);

	//	this->qbezs.push_back(seg);

	//	qbezsBuf.pop_front();
	//}

	//if (this->qbezs.size() > 100) {
	//	this->qbezs.pop_front();
	//}

	auto ptTmp = this->projection2DInv.TransformPoint(D2D1::Point2F(pt.x, pt.y));

	this->points.push_back(ptTmp);

	if (this->points.size() > 1000) {
		this->points.pop_front();
	}


}

void PlayerRenderer::PointerReleased(const DirectX::XMFLOAT2 &pt) {
	thread::critical_section::scoped_lock lk(this->cs);
}

void PlayerRenderer::DrawMovingLine(ID2D1SolidColorBrush *brush) {
	auto d2dFactory = this->dxDev->GetD2DFactory();
	auto d2dCtx = this->dxDev->D2D();

	std::vector<D2D1_POINT_2F> pts2, ptsOrig(this->points.begin(), this->points.end());

	pts2.reserve(this->points.size());

	float strokeWidth = 10.0f;

	bool restartVec = true;
	bool buildVec = false;
	bool estimateVec = false;
	float maxAngle = DirectX::XMConvertToRadians(20.0f);
	int beginCount = 2;
	float estimateLength = 30.0f;
	int beginReady = 0;
	DirectX::XMVECTOR beginVec, nextVec;
	DirectX::XMVECTOR startPt;

	std::vector<D2D1_QUADRATIC_BEZIER_SEGMENT> qvadratik;
	std::vector<D2D1_BEZIER_SEGMENT> bezs;
	std::vector<D2D1_POINT_2F> lines;

	// index, type
	// types : 0 - line, 1 - kvadratik, 2 - bez
	std::vector<std::pair<int, int>> segments;

	for (size_t i = 0; i < ptsOrig.size(); i++) {
		auto pt = this->projection2D.TransformPoint(ptsOrig[i]);

		pts2.push_back(pt);

		if (restartVec) {
			beginReady = 0;
			restartVec = false;
			buildVec = true;
			estimateVec = false;
			beginVec = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

			startPt = DirectX::XMVectorSet(pts2[pts2.size() - 1].x, pts2[pts2.size() - 1].y, 0.0f, 1.0f);
			continue;
		}

		if (buildVec) {
			auto pt0 = DirectX::XMVectorSet(pts2[pts2.size() - 2].x, pts2[pts2.size() - 2].y, 0.0f, 1.0f);
			auto pt1 = DirectX::XMVectorSet(pts2[pts2.size() - 1].x, pts2[pts2.size() - 1].y, 0.0f, 1.0f);

			auto vec = DirectX::XMVectorSubtract(pt1, pt0);
			beginVec = DirectX::XMVectorAdd(beginVec, vec);

			auto len = DirectX::XMVector2Length(beginVec).XF;

			beginReady++;

			if (len >= beginCount) {
				nextVec = beginVec;

				restartVec = false;
				buildVec = false;
				estimateVec = true;

				if (len >= estimateLength) {
					restartVec = true;
					buildVec = false;
					estimateVec = false;

					lines.push_back(D2D1::Point2F(pt1.XF, pt1.YF));
					segments.push_back(std::make_pair((int)lines.size() - 1, 0));
				}
			}

			continue;
		}

		if (estimateVec) {
			auto pt0 = DirectX::XMVectorSet(pts2[pts2.size() - 2].x, pts2[pts2.size() - 2].y, 0.0f, 1.0f);
			auto pt1 = DirectX::XMVectorSet(pts2[pts2.size() - 1].x, pts2[pts2.size() - 1].y, 0.0f, 1.0f);

			auto vec = DirectX::XMVectorSubtract(pt1, pt0);
			nextVec = DirectX::XMVectorAdd(nextVec, vec);

			auto len = DirectX::XMVector2Length(nextVec).XF;
			auto angle = std::fabsf(H::Math::GetAngleBetweenVectors(beginVec, nextVec));

			if (len > estimateLength || angle > maxAngle) {
				restartVec = true;
				buildVec = false;
				estimateVec = false;

				auto ttt0 = DirectX::XMConvertToRadians(0.125f);
				if (angle > ttt0) {
					DirectX::XMVECTOR endVec = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

					for (auto it = pts2.rbegin(); it != pts2.rend(); ++it) {
						auto pt0 = DirectX::XMVectorSet(it->x, it->y, 0.0f, 1.0f);
						++it;

						if (it != pts2.rend()) {
							auto pt1 = DirectX::XMVectorSet(it->x, it->y, 0.0f, 1.0f);
							auto vec = DirectX::XMVectorSubtract(pt1, pt0);
							endVec = DirectX::XMVectorAdd(endVec, vec);

							auto len = DirectX::XMVector2Length(endVec).XF;

							if (len >= beginCount) {
								break;
							}
						}
						else {
							break;
						}
					}

					if (endVec.XF == 0.0f && endVec.YF == 0.0f)
					{
						lines.push_back(D2D1::Point2F(pt1.XF, pt1.YF));
						segments.push_back(std::make_pair((int)lines.size() - 1, 0));
					}
					else {
						endVec = DirectX::XMVector2Normalize(endVec);
						beginVec = DirectX::XMVector2Normalize(beginVec);
						float beginEndAngle = std::fabsf(H::Math::GetAngleBetweenVectors(DirectX::XMVector2Normalize(endVec), DirectX::XMVector2Normalize(beginVec)));

						float maxAngle = 20.0f;
						auto mrotate = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(90.0f));
						auto mrotate2 = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(maxAngle));
						auto vec = DirectX::XMVector2Normalize(DirectX::XMVector3TransformNormal(beginVec, mrotate));

						if (std::fabsf(beginEndAngle) > DirectX::XMConvertToRadians(maxAngle)) {
							endVec = DirectX::XMVector2Normalize(DirectX::XMVector3TransformNormal(vec, mrotate2));
						}

						float beginEndAngle2 = std::fabsf(H::Math::GetAngleBetweenVectors(endVec, vec));

						auto plane = DirectX::XMPlaneFromPointNormal(startPt, DirectX::XMVector2Normalize(vec));
						auto endPt0 = pt1;
						auto endPt1 = DirectX::XMVectorAdd(endPt0, endVec);

						auto midPt = DirectX::XMPlaneIntersectLine(plane, endPt0, endPt1);

						if (midPt.XF > 2000 || midPt.YF > 2000) {
							int stop = 324;
						}

						qvadratik.push_back(D2D1::QuadraticBezierSegment(D2D1::Point2F(midPt.XF, midPt.YF), D2D1::Point2F(endPt0.XF, endPt0.YF)));
						segments.push_back(std::make_pair((int)qvadratik.size() - 1, 1));
					}

					beginVec = endVec;
					nextVec = endVec;
					restartVec = false;
					buildVec = false;
					estimateVec = true;
				}
				else {
					lines.push_back(D2D1::Point2F(pt1.XF, pt1.YF));
					segments.push_back(std::make_pair((int)lines.size() - 1, 0));
				}
				//pts2.clear();
			}

			continue;
		}

		//if (i == 0) {
		//	pts2.push_back(pt);
		//}
		//else if (i == ptsOrig.size() - 1) {
		//	pts2.push_back(pt);
		//}
		//else {
		//	auto prevPt = pts2[pts2.size() - 1];

		//	auto tmp0 = DirectX::XMVectorSet(prevPt.x, prevPt.y, 0.0f, 1.0f);
		//	auto tmp1 = DirectX::XMVectorSet(pt.x, pt.y, 0.0f, 1.0f);

		//	auto vec = DirectX::XMVectorSubtract(tmp1, tmp0);
		//	auto len = DirectX::XMVector2Length(vec).XF;

		//	/*if (len >= strokeWidth) {
		//		pts2.push_back(pt);
		//	}*/

		//	if (len >= 2) 
		//	{
		//		pts2.push_back(pt);
		//	}
		//}
	}

	if (pts2.size() > 1)
	{
		Microsoft::WRL::ComPtr<ID2D1PathGeometry> geom, geom2;
		Microsoft::WRL::ComPtr<ID2D1GeometrySink> sink, sink2;

		d2dFactory->CreatePathGeometry(geom.GetAddressOf());
		d2dFactory->CreatePathGeometry(geom2.GetAddressOf());

		geom->Open(sink.GetAddressOf());

		sink->BeginFigure(*pts2.begin(), D2D1_FIGURE_BEGIN_FILLED);
		auto it = pts2.begin();
		++it;

		for (; it != pts2.end(); ++it) {
			sink->AddLine(*it);
		}

		/*for (auto i : segments) {
		switch (i.second) {
		case 0:
		sink->AddLine(lines[i.first]);
		break;
		case 1:
		sink->AddQuadraticBezier(qvadratik[i.first]);
		break;
		case 2:
		sink->AddBezier(bezs[i.first]);
		break;
		default:
		break;
		}
		}

		if (estimateVec) {
		sink->AddLine(pts2.back());
		}*/

		sink->EndFigure(D2D1_FIGURE_END_OPEN);
		sink->Close();

		D2D1_STROKE_STYLE_PROPERTIES strokeStyle;
		Microsoft::WRL::ComPtr<ID2D1StrokeStyle> stroke;

		strokeStyle.startCap = D2D1_CAP_STYLE_ROUND;
		strokeStyle.endCap = D2D1_CAP_STYLE_ROUND;
		strokeStyle.dashCap = D2D1_CAP_STYLE_ROUND;
		strokeStyle.lineJoin = D2D1_LINE_JOIN_ROUND;
		strokeStyle.miterLimit = 10.0f;
		strokeStyle.dashStyle = D2D1_DASH_STYLE_SOLID;
		strokeStyle.dashOffset = 0.0f;

		d2dFactory->CreateStrokeStyle(&strokeStyle, nullptr, 0, stroke.GetAddressOf());

		d2dCtx->DrawGeometry(geom.Get(), brush, strokeWidth, stroke.Get());
	}

	//if (pts2.size() > 2) {
	//	auto ptIt = pts2.begin();
	//	Microsoft::WRL::ComPtr<ID2D1PathGeometry> geom;
	//	Microsoft::WRL::ComPtr<ID2D1PathGeometry> geom2;
	//	Microsoft::WRL::ComPtr<ID2D1GeometrySink> sink, sink2;

	//	d2dFactory->CreatePathGeometry(geom.GetAddressOf());
	//	d2dFactory->CreatePathGeometry(geom2.GetAddressOf());
	//	geom->Open(sink.GetAddressOf());
	//	sink->BeginFigure(*ptIt, D2D1_FIGURE_BEGIN_FILLED);
	//	++ptIt;

	//	while (ptIt != pts2.end()) {
	//		sink->AddLine(*ptIt);
	//		++ptIt;
	//	}

	//	sink->EndFigure(D2D1_FIGURE_END_OPEN);
	//	sink->Close();

	//	geom2->Open(sink2.GetAddressOf());
	//	geom->Simplify(D2D1_GEOMETRY_SIMPLIFICATION_OPTION_CUBICS_AND_LINES, D2D1::IdentityMatrix(), sink2.Get());
	//	sink2->Close();

	//	D2D1_STROKE_STYLE_PROPERTIES strokeStyle;
	//	Microsoft::WRL::ComPtr<ID2D1StrokeStyle> stroke;

	//	strokeStyle.startCap = D2D1_CAP_STYLE_ROUND;
	//	strokeStyle.endCap = D2D1_CAP_STYLE_ROUND;
	//	strokeStyle.dashCap = D2D1_CAP_STYLE_ROUND;
	//	strokeStyle.lineJoin = D2D1_LINE_JOIN_ROUND;
	//	strokeStyle.miterLimit = 10.0f;
	//	strokeStyle.dashStyle = D2D1_DASH_STYLE_SOLID;
	//	strokeStyle.dashOffset = 0.0f;

	//	d2dFactory->CreateStrokeStyle(&strokeStyle, nullptr, 0, stroke.GetAddressOf());

	//	d2dCtx->DrawGeometry(geom2.Get(), brush2.Get(), strokeWidth, stroke.Get());

	//	/*ptIt = pts2.begin();
	//	while (ptIt != pts2.end()) {
	//		d2dCtx->FillEllipse(D2D1::Ellipse(*ptIt, 5, 5), brush2.Get());
	//		++ptIt;
	//	}*/
	//}

	//if (pts2.size() > 3) {
	//	auto ptIt = pts2.begin();

	//	Microsoft::WRL::ComPtr<ID2D1PathGeometry> geom;
	//	Microsoft::WRL::ComPtr<ID2D1TransformedGeometry> geom2;
	//	Microsoft::WRL::ComPtr<ID2D1GeometrySink> sink;

	//	d2dFactory->CreatePathGeometry(geom.GetAddressOf());

	//	geom->Open(sink.GetAddressOf());

	//	sink->BeginFigure(*ptIt, D2D1_FIGURE_BEGIN_FILLED);

	//	while (ptIt != pts2.end()) {
	//		D2D1_POINT_2F pts[3];

	//		pts[0] = *ptIt;
	//		++ptIt;

	//		if (ptIt == pts2.end()) {
	//			sink->AddLine(pts[0]);
	//			break;
	//		}
	//		else {
	//			pts[1] = *ptIt;
	//			++ptIt;

	//			if (ptIt == pts2.end()) {
	//				D2D1_QUADRATIC_BEZIER_SEGMENT bez;

	//				bez.point1 = pts[0];
	//				bez.point2 = pts[1];

	//				sink->AddQuadraticBezier(bez);
	//				break;
	//			}
	//			else {
	//				D2D1_BEZIER_SEGMENT bez;

	//				pts[2] = *ptIt;

	//				bez.point1 = pts[0];
	//				bez.point2 = pts[1];
	//				bez.point3 = pts[2];

	//				sink->AddBezier(bez);
	//			}
	//		}

	//		//sink->AddLine(*ptIt);

	//		++ptIt;
	//	}

	//	sink->EndFigure(D2D1_FIGURE_END_OPEN);
	//	sink->Close();

	//	//d2dFactory->CreateTransformedGeometry(geom.Get(), this->projection2D, geom2.GetAddressOf());

	//	d2dCtx->DrawGeometry(geom.Get(), brush2.Get(), strokeWidth);
	//}

	//if (this->qbezs.size() > 1) {
	//	Microsoft::WRL::ComPtr<ID2D1PathGeometry> geom;
	//	Microsoft::WRL::ComPtr<ID2D1TransformedGeometry> geom2;
	//	Microsoft::WRL::ComPtr<ID2D1GeometrySink> sink;

	//	d2dFactory->CreatePathGeometry(geom.GetAddressOf());

	//	geom->Open(sink.GetAddressOf());

	//	auto ptIt = this->qbezs.begin();

	//	sink->BeginFigure(ptIt->point1, D2D1_FIGURE_BEGIN_FILLED);
	//	++ptIt;

	//	while (ptIt != this->qbezs.end()) {
	//		sink->AddBezier(*ptIt);

	//		++ptIt;
	//	}

	//	sink->EndFigure(D2D1_FIGURE_END_OPEN);
	//	sink->Close();

	//	//d2dFactory->CreateTransformedGeometry(geom.Get(), this->projection2D, geom2.GetAddressOf());

	//	d2dCtx->DrawGeometry(geom.Get(), brush2.Get(), strokeWidth);
	//}
}