#include "XMVectorBox.h"
#include "../HMath.h"

DirectX::BoundingBox XMVectorBox::GetBoundingBox() const {
	DirectX::BoundingBox aabb;
	DirectX::XMVECTOR center = DirectX::XMVectorScale(DirectX::XMVectorAdd(this->RightBottom, this->LeftTop), 0.5f);
	DirectX::XMVECTOR size = DirectX::XMVectorScale(DirectX::XMVectorSubtract(this->RightBottom, this->LeftTop), 0.5f);

	aabb.Transform(aabb,
		DirectX::XMMatrixMultiply(
			DirectX::XMMatrixScaling(size.XF, size.YF, 1.0f),
			DirectX::XMMatrixTranslation(center.XF, center.YF, 0.0f)));

	return aabb;
}