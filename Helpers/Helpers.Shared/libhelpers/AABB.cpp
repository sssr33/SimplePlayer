#if INCLUDE_PCH_H == 1
#include "pch.h"
#endif
#include "AABB.h"

#include <cmath>

AABB::AABB()
	: center(0, 0), radius(0, 0)
{
}

AABB::AABB(const D2D1_RECT_F &bbox) {
	this->center.x = bbox.left + (bbox.right - bbox.left) / 2;
	this->center.y = bbox.top + (bbox.bottom - bbox.top) / 2;

	this->radius.x = (bbox.right - bbox.left) / 2;
	this->radius.y = (bbox.bottom - bbox.top) / 2;
}

AABB::AABB(const AABBLeftTopSize &key, DirectX::CXMVECTOR &leftTop, DirectX::CXMVECTOR &size) {
	auto radiusTmp = DirectX::XMVectorScale(size, 0.5f);
	auto centerTmp = DirectX::XMVectorAdd(leftTop, radiusTmp);

	DirectX::XMStoreFloat2(&this->radius, radiusTmp);
	DirectX::XMStoreFloat2(&this->center, centerTmp);
}

AABB::AABB(const AABBLeftTopRightBottom &key, DirectX::CXMVECTOR &leftTop, DirectX::CXMVECTOR &rightBottom)
	: AABB(AABBLeftTopSize(), leftTop, DirectX::XMVectorSubtract(rightBottom, leftTop))
{
}

AABB::AABB(const AABBLeftTopSize &key, const DirectX::XMFLOAT2 &leftTop, const DirectX::XMFLOAT2 &size) {
	this->radius.x = size.x * 0.5f;
	this->radius.y = size.y * 0.5f;

	this->center.x = leftTop.x + this->radius.x;
	this->center.y = leftTop.y + this->radius.y;
}

AABB::AABB(const AABBLeftTopSize &key, float left, float top, float width, float height) 
	: AABB(key, DirectX::XMFLOAT2(left, top), DirectX::XMFLOAT2(width, height))
{
}

bool AABB::DoCollide(const AABB &rect) {
	if (std::abs(this->center.x - rect.center.x) > (this->radius.x + rect.radius.x)) {
		return false;
	}

	if (std::abs(this->center.y - rect.center.y) > (this->radius.y + rect.radius.y)) {
		return false;
	}
	return true;
}

bool AABB::Contains(const AABB & rect)
{
	float rectL = rect.center.x - rect.radius.x;
	float rectT = rect.center.y - rect.radius.y;
	float rectR = rect.center.x + rect.radius.x;
	float rectB = rect.center.y + rect.radius.y;

	if (this->center.x - this->radius.x <= rectL &&
		this->center.y - this->radius.y <= rectT &&
		this->center.x + this->radius.x >= rectR &&
		this->center.y + this->radius.y >= rectB) {
		return true;
	}
	return false;
}

bool AABB::operator==(const AABB &other) const {
	if (this == &other) {
		return true;
	}

	bool centerSame = this->center.x == other.center.x && this->center.y == other.center.y;
	bool rediusSame = this->radius.x == other.radius.x && this->radius.y == other.radius.y;

	return centerSame && rediusSame;
}

bool AABB::operator!=(const AABB &other) const {
	return !this->operator==(other);
}

float AABB::GetArea() const {
	float area = 4.0f * this->radius.x * this->radius.y;
	return area;
}