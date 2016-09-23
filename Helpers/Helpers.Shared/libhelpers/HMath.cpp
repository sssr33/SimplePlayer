#include "HMath.h"

namespace H {
	const D2D1_MATRIX_3X2_F &MathIdentity<D2D1_MATRIX_3X2_F>::Get() {
		static const D2D1_MATRIX_3X2_F val = D2D1::IdentityMatrix();
		return val;
	}

	const D2D1::Matrix3x2F &MathIdentity<D2D1::Matrix3x2F>::Get() {
		static const D2D1::Matrix3x2F val = D2D1::Matrix3x2F::Identity();
		return val;
	}

	const DirectX::XMMATRIX &MathIdentity<DirectX::XMMATRIX>::Get() {
		static const DirectX::XMMATRIX val = DirectX::XMMatrixIdentity();
		return val;
	}

	const DirectX::XMFLOAT4X4 &MathIdentity<DirectX::XMFLOAT4X4>::Get() {
		static auto initFn = []() {
			DirectX::XMFLOAT4X4 res;
			DirectX::XMMATRIX tmp = DirectX::XMMatrixIdentity();

			DirectX::XMStoreFloat4x4(&res, tmp);

			return res;
		};
		static const DirectX::XMFLOAT4X4 val = initFn();
		return val;
	}




	DirectX::XMFLOAT2 Math::InscribeRect(const DirectX::XMFLOAT2 &sourRect, const DirectX::XMFLOAT2 &destRect) {
		DirectX::XMFLOAT2 inscribed = sourRect;
		bool widthLess = sourRect.x < destRect.x;
		bool heightLess = sourRect.y < destRect.y;
		bool hardCase = !widthLess && !heightLess;

		if (hardCase) {
			if (sourRect.x > sourRect.y) {
				float invAr = sourRect.y / sourRect.x;

				inscribed.x = destRect.x;
				inscribed.y = destRect.x * invAr;

				if (inscribed.y > destRect.y) {
					float hscale = destRect.y / inscribed.y;

					inscribed.x *= hscale;
					inscribed.y *= hscale;
				}
			}
			else {
				float ar = sourRect.x / sourRect.y;

				inscribed.x = destRect.y * ar;
				inscribed.y = destRect.y;

				if (inscribed.x > destRect.x) {
					float wscale = destRect.x / inscribed.x;

					inscribed.x *= wscale;
					inscribed.y *= wscale;
				}
			}
		}
		else {
			if (widthLess && !heightLess) {
				float hscale = destRect.y / sourRect.y;

				inscribed.x *= hscale;
				inscribed.y *= hscale;
			}

			if (!widthLess && heightLess) {
				float wscale = destRect.x / sourRect.x;

				inscribed.x *= wscale;
				inscribed.y *= wscale;
			}
		}

		return inscribed;
	}

	void Math::BoundingOrientedBoxTransform(const DirectX::BoundingOrientedBox &_this, DirectX::BoundingOrientedBox &Out, DirectX::CXMMATRIX M) {
		// Load the box.
		DirectX::XMVECTOR vCenter = DirectX::XMLoadFloat3(&_this.Center);
		DirectX::XMVECTOR vExtents = DirectX::XMLoadFloat3(&_this.Extents);
		DirectX::XMVECTOR vOrientation = DirectX::XMLoadFloat4(&_this.Orientation);

		assert(DirectX::Internal::XMQuaternionIsUnit(vOrientation));

		// Composite the box rotation and the transform rotation.
		DirectX::XMMATRIX nM;
		nM.r[0] = DirectX::XMVector3Normalize(M.r[0]);
		nM.r[1] = DirectX::XMVector3Normalize(M.r[1]);
		nM.r[2] = DirectX::XMVector3Normalize(M.r[2]);
		nM.r[3] = DirectX::g_XMIdentityR3;
		DirectX::XMVECTOR Rotation = DirectX::XMQuaternionRotationMatrix(nM);
		vOrientation = DirectX::XMQuaternionMultiply(vOrientation, Rotation);

		// Transform the center.
		vCenter = DirectX::XMVector3Transform(vCenter, M);

		// Scale the box extents.
		DirectX::XMVECTOR dX = DirectX::XMVector3Length(M.r[0]);
		DirectX::XMVECTOR dY = DirectX::XMVector3Length(M.r[1]);
		DirectX::XMVECTOR dZ = DirectX::XMVector3Length(M.r[2]);

		// bug:
		/*XMVECTOR VectorScale = XMVectorSelect(dX, dY, g_XMSelect1000);
		VectorScale = XMVectorSelect(VectorScale, dZ, g_XMSelect1100);*/
		// fix:
		DirectX::XMVECTOR VectorScale = DirectX::XMVectorSelect(dY, dX, DirectX::g_XMSelect1000);
		VectorScale = DirectX::XMVectorSelect(dZ, VectorScale, DirectX::g_XMSelect1100);
		vExtents = DirectX::XMVectorMultiply(vExtents, VectorScale);
		// Store the box.
		DirectX::XMStoreFloat3(&Out.Center, vCenter);
		DirectX::XMStoreFloat3(&Out.Extents, vExtents);
		DirectX::XMStoreFloat4(&Out.Orientation, vOrientation);
	}

	DirectX::ContainmentType Math::BoundingBoxContains(const DirectX::BoundingBox &_this, const DirectX::BoundingOrientedBox& box) {
		if (!box.Intersects(_this))
			return DirectX::ContainmentType::DISJOINT;

		DirectX::XMVECTOR vCenter = DirectX::XMLoadFloat3(&_this.Center);
		DirectX::XMVECTOR vExtents = DirectX::XMLoadFloat3(&_this.Extents);

		// Subtract off the AABB center to remove a subtract below
		DirectX::XMVECTOR oCenter = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&box.Center), vCenter);

		DirectX::XMVECTOR oExtents = DirectX::XMLoadFloat3(&box.Extents);
		DirectX::XMVECTOR oOrientation = DirectX::XMLoadFloat4(&box.Orientation);

		assert(DirectX::Internal::XMQuaternionIsUnit(oOrientation));

		DirectX::XMVECTOR Inside = DirectX::XMVectorTrueInt();

		for (size_t i = 0; i < DirectX::BoundingOrientedBox::CORNER_COUNT; ++i)
		{
			DirectX::XMVECTOR C = DirectX::XMVectorAdd(DirectX::XMVector3Rotate(DirectX::XMVectorMultiply(oExtents, DirectX::g_BoxOffset[i]), oOrientation), oCenter);
			DirectX::XMVECTOR d = DirectX::XMVectorAbs(C);
			Inside = DirectX::XMVectorAndInt(Inside, DirectX::XMVectorLessOrEqual(d, vExtents));
		}

		return (DirectX::XMVector3EqualInt(Inside, DirectX::XMVectorTrueInt())) ? DirectX::ContainmentType::CONTAINS : DirectX::ContainmentType::INTERSECTS;
	}

	DirectX::XMMATRIX Math::XMMatrixFrom(const D2D1_MATRIX_3X2_F &v) {
		DirectX::XMMATRIX m = DirectX::XMMatrixIdentity();

		m.r[0].XF = v._11;
		m.r[0].YF = v._12;

		m.r[1].XF = v._21;
		m.r[1].YF = v._22;

		m.r[3].XF = v._31;
		m.r[3].YF = v._32;

		return m;
	}

	D2D1::Matrix3x2F Math::D2D1Matrix3X2From(DirectX::CXMMATRIX v) {
		D2D1::Matrix3x2F m;

		m._11 = v.r[0].XF;
		m._12 = v.r[0].YF;

		m._21 = v.r[1].XF;
		m._22 = v.r[1].YF;

		m._31 = v.r[3].XF;
		m._32 = v.r[3].YF;

		return m;
	}

	D2D1::Matrix3x2F Math::D2D1Matrix3X2From(const DirectX::XMFLOAT4X4 &v) {
		D2D1::Matrix3x2F m;

		m._11 = v.m[0][0];
		m._12 = v.m[0][1];

		m._21 = v.m[1][0];
		m._22 = v.m[1][1];

		m._31 = v.m[3][0];
		m._32 = v.m[3][1];

		return m;
	}

	DirectX::XMVECTOR Math::GetAABBSizeFrom(DirectX::CXMMATRIX matrix) {
		DirectX::XMVECTOR axisX = DirectX::XMVectorAbs(matrix.r[0]);
		DirectX::XMVECTOR axisY = DirectX::XMVectorAbs(matrix.r[1]);
		DirectX::XMVECTOR axisZ = DirectX::XMVectorAbs(matrix.r[2]);

		DirectX::XMVECTOR addXY = DirectX::XMVectorAdd(axisX, axisY);
		DirectX::XMVECTOR addXYZ = DirectX::XMVectorAdd(addXY, axisZ);

		return addXYZ;
	}

	int Math::RoundUp(int numToRound, int multiple) {
		if (multiple == 0)
		{
			return numToRound;
		}

		int remainder = numToRound % multiple;
		if (remainder == 0)
		{
			return numToRound;
		}

		return numToRound + multiple - remainder;
	}

	float Math::GetRadsToDegsScale() {
		const float scale = (180.0f / DirectX::XM_PI);
		return scale;
	}

	float Math::GetDegsToRadsScale() {
		const float scale = (DirectX::XM_PI / 180.0f);
		return scale;
	}

	float Math::GetAngleBetweenLines(DirectX::CXMVECTOR p00, DirectX::CXMVECTOR p01, DirectX::CXMVECTOR p10, DirectX::CXMVECTOR p11) {
		auto p0 = DirectX::XMVectorSubtract(p01, p00);
		auto p1 = DirectX::XMVectorSubtract(p11, p10);

		return Math::GetAngleBetweenVectors(p0, p1);
	}

	float Math::GetAngleBetweenVectors(DirectX::CXMVECTOR p0, DirectX::CXMVECTOR p1, float *angle0, float *angle1) {
		auto x = DirectX::XMVectorPermute<0, 4, 0, 0>(p0, p1);
		auto y = DirectX::XMVectorPermute<1, 5, 0, 0>(p0, p1);

		auto tmp = DirectX::XMVectorATan2(y, x);

		if (angle0) {
			*angle0 = tmp.F32[0];
		}

		if (angle1) {
			*angle1 = tmp.F32[1];
		}

		float angle = tmp.F32[1] - tmp.F32[0];
		return angle;
	}

	DirectX::XMFLOAT3 Math::CMYKtoRGB(const DirectX::XMFLOAT4 cmyk) {
		DirectX::XMFLOAT3 res;

		res.x = (1.0f - cmyk.x) * (1.0f - cmyk.w);
		res.y = (1.0f - cmyk.y) * (1.0f - cmyk.w);
		res.z = (1.0f - cmyk.z) * (1.0f - cmyk.w);

		return res;
	}

	void Math::Concat(const D2D1_RECT_F &a, const D2D1_RECT_F &b, D2D1_RECT_F &result) {
		if (a.right >= a.left && a.bottom >= a.top &&
			b.right >= b.left && b.bottom >= b.top)
		{
			D2D1_RECT_F resultTmp;

			resultTmp.left = (std::min)(a.left, b.left);
			resultTmp.top = (std::min)(a.top, b.top);
			resultTmp.right = (std::max)(a.right, b.right);
			resultTmp.bottom = (std::max)(a.bottom, b.bottom);

			result = resultTmp;
		}
	}

	DirectX::XMVECTOR Math::ToCameraSpace(
		DirectX::CXMMATRIX projection,
		DirectX::CXMMATRIX camera,
		const DirectX::XMFLOAT2 &screenPos,
		const DirectX::XMFLOAT2 &screenSize,
		float cameraZ,
		float resultZ)
	{
		DirectX::XMVECTOR res;
		auto tmpPt = DirectX::XMVectorSet(0, 0, resultZ - cameraZ, 1);

		tmpPt = DirectX::XMVector3TransformCoord(tmpPt, projection);

		DirectX::XMVECTOR pt = DirectX::XMVectorSet(screenPos.x, screenPos.y, tmpPt.ZF, 1.0f);

		res = DirectX::XMVector3Unproject(
			pt,
			0.0f, 0.0f, screenSize.x, screenSize.y,
			0.0f, 1.0f,
			projection,
			camera,
			DirectX::XMMatrixIdentity());


		return res;
	}

	DirectX::XMVECTOR Math::Signum(DirectX::CXMVECTOR v) {
		auto cmp1 = DirectX::XMVectorLess(DirectX::g_XMZero, v);
		auto cmp2 = DirectX::XMVectorLess(v, DirectX::g_XMZero);

		auto v1 = DirectX::XMVectorSelect(DirectX::g_XMZero, DirectX::g_XMOne, cmp1);
		auto v2 = DirectX::XMVectorSelect(DirectX::g_XMZero, DirectX::g_XMOne, cmp2);

		auto signum = DirectX::XMVectorSubtract(v1, v2);
		return signum;
	}

	DirectX::XMVECTOR Math::Sign(DirectX::CXMVECTOR v) {
		auto mask = DirectX::XMVectorAndInt(v, DirectX::XMVectorSplatSignMask());
		auto sign = DirectX::XMVectorOrInt(DirectX::g_XMOne, mask);

		return sign;
	}

	int Math::GetIntersectingFactor(const D2D1_RECT_F& annotRect, const DirectX::XMFLOAT2& pageDimensions)
	{
		int factor = 0;

		factor = -1 * (annotRect.bottom <= 0 ||
			annotRect.top <= 0 ||
			annotRect.right >= pageDimensions.x ||
			annotRect.left >= pageDimensions.x ||
			annotRect.bottom >= pageDimensions.y ||
			annotRect.top >= pageDimensions.y);

		factor += annotRect.right <= 0 ||
			annotRect.left <= 0;

		return factor;
	}

	void Math::InscribePointInRect(D2D1_POINT_2F &point, const D2D1_RECT_F& rect)
	{
		if (point.x < rect.left)
			point.x = rect.left;
		if (point.x > rect.right)
			point.x = rect.right;

		if (point.y < rect.top)
			point.y = rect.top;
		if (point.y > rect.bottom)
			point.y = rect.bottom;
	}

	void Math::InscribeRectInRect(D2D1_RECT_F &rect, const D2D1_RECT_F& targetRect)
	{
		if (rect.left < targetRect.left)
			rect.left = targetRect.left;

		if (rect.right < targetRect.left)
			rect.right = targetRect.left;

		if (rect.left > targetRect.right)
			rect.left = targetRect.right;

		if (rect.right > targetRect.right)
			rect.right = targetRect.right;

		if (rect.top < targetRect.top)
			rect.top = targetRect.top;

		if (rect.bottom < targetRect.top)
			rect.bottom = targetRect.top;

		if (rect.top > targetRect.bottom)
			rect.top = targetRect.bottom;

		if (rect.bottom > targetRect.bottom)
			rect.bottom = targetRect.bottom;
	}

	DirectX::XMFLOAT2 Math::InscribeRectAR(const DirectX::XMFLOAT2 &dest, const DirectX::XMFLOAT2 &src) {
		DirectX::XMFLOAT2 inscribed;
		float destAr = dest.x / dest.y;
		float srcAr = src.x / src.y;

		if (srcAr > destAr) {
			float scale = destAr / srcAr;

			inscribed.x = dest.x;
			inscribed.y = dest.y * scale;
		}
		else {
			inscribed.x = dest.y * srcAr;
			inscribed.y = dest.y;
		}

		return inscribed;
	}

	DirectX::XMFLOAT2 Math::GetScale(const D2D1_MATRIX_3X2_F &v) {
		DirectX::XMFLOAT2 scale;
		DirectX::XMVECTOR xVec = DirectX::XMVectorSet(v._11, v._12, 0.0f, 0.0f);
		DirectX::XMVECTOR yVec = DirectX::XMVectorSet(v._21, v._22, 0.0f, 0.0f);

		scale.x = DirectX::XMVector2Length(xVec).XF;
		scale.y = DirectX::XMVector2Length(yVec).XF;

		return scale;
	}

	void Math::GetComponents(
		const D2D1_MATRIX_3X2_F &v,
		DirectX::XMFLOAT2 &scale,
		DirectX::XMFLOAT2 &xVec,
		DirectX::XMFLOAT2 &yVec)
	{
		DirectX::XMVECTOR xVecTmp = DirectX::XMVectorSet(v._11, v._12, 0.0f, 0.0f);
		DirectX::XMVECTOR yVecTmp = DirectX::XMVectorSet(v._21, v._22, 0.0f, 0.0f);

		scale.x = DirectX::XMVector2Length(xVecTmp).XF;
		scale.y = DirectX::XMVector2Length(yVecTmp).XF;

		// normalize
		xVecTmp = DirectX::XMVectorScale(xVecTmp, 1.0f / scale.x);
		yVecTmp = DirectX::XMVectorScale(yVecTmp, 1.0f / scale.y);

		DirectX::XMStoreFloat2(&xVec, xVecTmp);
		DirectX::XMStoreFloat2(&yVec, yVecTmp);
	}

	void Math::SetComponents(
		D2D1_MATRIX_3X2_F &v,
		const DirectX::XMFLOAT2 &scale,
		const DirectX::XMFLOAT2 &xVec,
		const DirectX::XMFLOAT2 &yVec)
	{
		auto xVecTmp = DirectX::XMLoadFloat2(&xVec);
		auto yVecTmp = DirectX::XMLoadFloat2(&yVec);

		xVecTmp = DirectX::XMVectorScale(xVecTmp, scale.x);
		yVecTmp = DirectX::XMVectorScale(yVecTmp, scale.y);

		v._11 = xVecTmp.XF;
		v._12 = xVecTmp.YF;

		v._21 = yVecTmp.XF;
		v._22 = yVecTmp.YF;
	}

	uint32_t Math::NextPowerOf2(uint32_t v) {
		v--;
		v |= v >> 1;
		v |= v >> 2;
		v |= v >> 4;
		v |= v >> 8;
		v |= v >> 16;
		v++;
		return v;
	}

	DirectX::XMFLOAT2 Math::FitObjectSize(
		const DirectX::XMFLOAT2 &destSize,
		const DirectX::XMFLOAT2 &srcSize,
		FitObjectSizeMode mode)
	{
		DirectX::XMFLOAT2 fitted;

		switch (mode) {
		case FitObjectSizeMode::MaxSide: {
			fitted = H::Math::InscribeRectAR(destSize, srcSize);
			break;
		}
		case FitObjectSizeMode::Height: {
			float heightFactor = destSize.y / srcSize.y;

			fitted.x = srcSize.x * heightFactor;
			fitted.y = srcSize.y * heightFactor;
			break;
		}
		case FitObjectSizeMode::MinSide: {
			auto tmp = H::Math::InscribeRectAR(srcSize, destSize);
			float scale = destSize.x / tmp.x;

			fitted.x = srcSize.x * scale;
			fitted.y = srcSize.y * scale;
			break;
		}
		default:
			break;
		}

		return fitted;
	}

	D2D1_RECT_F Math::TransformRect(const D2D1_RECT_F &rect, const D2D1_MATRIX_3X2_F &matrix) {
		D2D1_RECT_F transformed;
		auto leftTop = D2D1::Point2F(rect.left, rect.top);
		auto rightBottom = D2D1::Point2F(rect.right, rect.bottom);
		auto matrixTmp = D2D1::Matrix3x2F::ReinterpretBaseType(&matrix);

		leftTop = matrixTmp->TransformPoint(leftTop);
		rightBottom = matrixTmp->TransformPoint(rightBottom);

		transformed.left = leftTop.x;
		transformed.top = leftTop.y;

		transformed.right = rightBottom.x;
		transformed.bottom = rightBottom.y;

		return transformed;
	}

	D2D1_RECT_F Math::SortRect(const D2D1_RECT_F &rect) {
		D2D1_RECT_F sorted;

		sorted.left = (std::min)(rect.left, rect.right);
		sorted.top = (std::min)(rect.top, rect.bottom);

		sorted.right = (std::max)(rect.left, rect.right);
		sorted.bottom = (std::max)(rect.top, rect.bottom);

		return sorted;
	}

	D2D1_RECT_U Math::RoundRect(const D2D1_RECT_F &rect) {
		D2D1_RECT_U res;

		res.left = (uint32_t)std::floorf(rect.left);
		res.top = (uint32_t)std::floorf(rect.top);

		res.right = (uint32_t)std::ceilf(rect.right);
		res.bottom = (uint32_t)std::ceilf(rect.bottom);

		return res;
	}

	bool Math::RectContains(const D2D1_RECT_F &rect, const D2D1_POINT_2F &point) {
		bool contains = false;
		auto tmpRect = Math::SortRect(rect);

		if (point.x >= tmpRect.left && point.x <= tmpRect.right &&
			point.y >= tmpRect.top && point.y <= tmpRect.bottom)
		{
			contains = true;
		}

		return contains;
	}
}