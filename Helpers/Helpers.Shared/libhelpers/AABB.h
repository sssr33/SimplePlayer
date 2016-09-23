#pragma once

#include <d2d1.h>
#include <DirectXMath.h>

struct AABBLeftTopRightBottom {};
struct AABBLeftTopSize {};

class AABB {
public:
	AABB();
	AABB(const D2D1_RECT_F &bbox);
	AABB(const AABBLeftTopSize &key, DirectX::CXMVECTOR &leftTop, DirectX::CXMVECTOR &size);
	AABB(const AABBLeftTopRightBottom &key, DirectX::CXMVECTOR &leftTop, DirectX::CXMVECTOR &rightBottom);
	AABB(const AABBLeftTopSize &key, const DirectX::XMFLOAT2 &leftTop, const DirectX::XMFLOAT2 &size);
	AABB(const AABBLeftTopSize &key, float left, float top, float width, float height);
	bool DoCollide(const AABB &rect);
	bool Contains(const AABB& rect);
	DirectX::XMFLOAT2 Center() { return center; }
	DirectX::XMFLOAT2 Radius() { return radius; }

	bool operator==(const AABB &other) const;
	bool operator!=(const AABB &other) const;

	float GetArea() const;

private:
	DirectX::XMFLOAT2 center, radius;
};