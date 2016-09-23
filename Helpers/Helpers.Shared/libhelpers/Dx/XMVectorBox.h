#pragma once

#include <DirectXMath.h>
#include <DirectXCollision.h>

class XMVectorBox {
public:
	DirectX::XMVECTOR LeftTop;
	DirectX::XMVECTOR RightBottom;

	DirectX::BoundingBox GetBoundingBox() const;
};