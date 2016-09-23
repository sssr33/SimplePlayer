#pragma once
#include "HMathCP.h"

#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <d2d1.h>
#include <algorithm>

// Defines for accessing XMVECTOR components without XMVectorGet.. , XMVectorSet.. methods
#ifdef _M_ARM

#define XF n128_f32[0]
#define YF n128_f32[1]
#define ZF n128_f32[2]
#define WF n128_f32[3]

#define XU32 n128_u32[0]
#define YU32 n128_u32[1]
#define ZU32 n128_u32[2]
#define WU32 n128_u32[3]

#define XI32 n128_i32[0]
#define YI32 n128_i32[1]
#define ZI32 n128_i32[2]
#define WI32 n128_i32[3]

#define F32 n128_f32

#define I8 n128_i8
#define I16 n128_i16
#define I32 n128_i32
#define I64 n128_i64

#define U8 n128_u8
#define U16 n128_u16
#define U32 n128_u32
#define U64 n128_u64

#else

#define XF m128_f32[0]
#define YF m128_f32[1]
#define ZF m128_f32[2]
#define WF m128_f32[3]

#define XU32 m128_u32[0]
#define YU32 m128_u32[1]
#define ZU32 m128_u32[2]
#define WU32 m128_u32[3]

#define XI32 m128_i32[0]
#define YI32 m128_i32[1]
#define ZI32 m128_i32[2]
#define WI32 m128_i32[3]

#define F32 m128_f32

#define I8 m128_i8
#define I16 m128_i16
#define I32 m128_i32
#define I64 m128_i64

#define U8 m128_u8
#define U16 m128_u16
#define U32 m128_u32
#define U64 m128_u64

#endif

namespace H {
	enum class FitObjectSizeMode {
		MaxSide,
		MinSide,
		Height
	};

	// class that returns references to variables with identity values
	template<class T>
	struct MathIdentity {
		static const T &Get() {
			static_assert(false, "Not implemented");
		}
	};

	template<>
	struct MathIdentity<D2D1_MATRIX_3X2_F> {
		static const D2D1_MATRIX_3X2_F &Get();
	};

	template<>
	struct MathIdentity<D2D1::Matrix3x2F> {
		static const D2D1::Matrix3x2F &Get();
	};

	template<>
	struct MathIdentity<DirectX::XMMATRIX> {
		static const DirectX::XMMATRIX &Get();
	};

	template<>
	struct MathIdentity<DirectX::XMFLOAT4X4> {
		static const DirectX::XMFLOAT4X4 &Get();
	};

	class Math : public MathCP {
	public:
		/// <summary>
		/// Inscribes source rectangle into destination rectangle.
		/// If source rectangle is bigger than destination one then
		/// inscribed will be rectangle with maximum size that can
		/// be in destination rectangle and same aspect ratio as source rectangle.
		/// </summary>
		/// <param name="sourRect">Rectangle that need to inscribed.</param>
		/// <param name="destRect">Rectangle in which sourRect will bee inscribed.</param>
		/// <returns>Returns inscribed rectangle.</returns>
		static DirectX::XMFLOAT2 InscribeRect(const DirectX::XMFLOAT2 &sourRect, const DirectX::XMFLOAT2 &destRect);

		/*
		DirectXMath have bug in BoundingOrientedBox::Transform method:
		http://xboxforums.create.msdn.com/forums/p/113061/680807.aspx
		This implementation have fix for bug.
		*/
		static void BoundingOrientedBoxTransform(const DirectX::BoundingOrientedBox &_this, DirectX::BoundingOrientedBox &Out, DirectX::CXMMATRIX M);

		static DirectX::ContainmentType BoundingBoxContains(const DirectX::BoundingBox &_this, const DirectX::BoundingOrientedBox& box);

		static DirectX::XMMATRIX XMMatrixFrom(const D2D1_MATRIX_3X2_F &v);

		static D2D1::Matrix3x2F D2D1Matrix3X2From(DirectX::CXMMATRIX v);
		static D2D1::Matrix3x2F D2D1Matrix3X2From(const DirectX::XMFLOAT4X4 &v);

		static DirectX::XMVECTOR GetAABBSizeFrom(DirectX::CXMMATRIX matrix);

		static int RoundUp(int numToRound, int multiple);

		static float GetRadsToDegsScale();

		static float GetDegsToRadsScale();

		static float GetAngleBetweenLines(DirectX::CXMVECTOR p00, DirectX::CXMVECTOR p01, DirectX::CXMVECTOR p10, DirectX::CXMVECTOR p11);

		static float GetAngleBetweenVectors(DirectX::CXMVECTOR p0, DirectX::CXMVECTOR p1, float *angle0 = nullptr, float *angle1 = nullptr);

		// cmyk in range 0.0 1.0
		// rgb in range 0.0 1.0
		// rgb = xyz
		// cmyk = xyzw
		// based on http://www.rapidtables.com/convert/color/cmyk-to-rgb.htm
		static DirectX::XMFLOAT3 CMYKtoRGB(const DirectX::XMFLOAT4 cmyk);

		static void Concat(const D2D1_RECT_F &a, const D2D1_RECT_F &b, D2D1_RECT_F &result);

		static DirectX::XMVECTOR ToCameraSpace(
			DirectX::CXMMATRIX projection,
			DirectX::CXMMATRIX camera,
			const DirectX::XMFLOAT2 &screenPos,
			const DirectX::XMFLOAT2 &screenSize,
			float cameraZ,
			float resultZ = 1.0f);

		static DirectX::XMVECTOR Signum(DirectX::CXMVECTOR v);

		// v < 0 = -1
		// v >= 0 = +1;
		static DirectX::XMVECTOR Sign(DirectX::CXMVECTOR v);

		static int GetIntersectingFactor(const D2D1_RECT_F& annotRect, const DirectX::XMFLOAT2& pageDimensions);

		static void InscribePointInRect(D2D1_POINT_2F &point, const D2D1_RECT_F& rect);

		static void InscribeRectInRect(D2D1_RECT_F &rect, const D2D1_RECT_F& targetRect);

		static DirectX::XMFLOAT2 InscribeRectAR(const DirectX::XMFLOAT2 &dest, const DirectX::XMFLOAT2 &src);

		static DirectX::XMFLOAT2 GetScale(const D2D1_MATRIX_3X2_F &v);

		static void GetComponents(
			const D2D1_MATRIX_3X2_F &v,
			DirectX::XMFLOAT2 &scale,
			DirectX::XMFLOAT2 &xVec,
			DirectX::XMFLOAT2 &yVec);

		static void SetComponents(
			D2D1_MATRIX_3X2_F &v,
			const DirectX::XMFLOAT2 &scale,
			const DirectX::XMFLOAT2 &xVec,
			const DirectX::XMFLOAT2 &yVec);

		static uint32_t NextPowerOf2(uint32_t v);

		static DirectX::XMFLOAT2 FitObjectSize(
			const DirectX::XMFLOAT2 &destSize,
			const DirectX::XMFLOAT2 &srcSize,
			FitObjectSizeMode mode);

		static D2D1_RECT_F TransformRect(const D2D1_RECT_F &rect, const D2D1_MATRIX_3X2_F &matrix);

		static D2D1_RECT_F SortRect(const D2D1_RECT_F &rect);

		static D2D1_RECT_U RoundRect(const D2D1_RECT_F &rect);

		static bool RectContains(const D2D1_RECT_F &rect, const D2D1_POINT_2F &point);

		template<class T>
		static const T &Identity() {
			return MathIdentity<T>::Get();
		}
	};
}

inline bool operator==(const D2D1::ColorF &a, const D2D1::ColorF &b) {
	bool res = a.a == b.a && a.r == b.r && a.g == b.g && a.b == b.b;
	return res;
}

inline bool operator!=(const D2D1::ColorF &a, const D2D1::ColorF &b) {
	bool res = !operator==(a, b);
	return res;
}

inline bool operator==(const DirectX::XMFLOAT2 &a, const DirectX::XMFLOAT2 &b) {
	bool res = a.x == b.x && a.y == b.y;
	return res;
}

inline bool operator!=(const DirectX::XMFLOAT2 &a, const DirectX::XMFLOAT2 &b) {
	bool res = !operator==(a, b);
	return res;
}

inline DirectX::XMFLOAT3 operator/ (const DirectX::XMFLOAT3& a,
	const DirectX::XMFLOAT3& b) {
	return DirectX::XMFLOAT3(a.x / b.x,
		a.y / b.y,
		a.z / b.z);
}