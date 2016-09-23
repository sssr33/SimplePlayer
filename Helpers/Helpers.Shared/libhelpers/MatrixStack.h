#pragma once
#include "AlignedAllocator.h"

#include <DirectXMath.h>
#include <d2d1.h>

class MatrixStack {
public:
	bool IsEmpty() const;

	const DirectX::XMMATRIX &GetD3DMatrix() const;
	const D2D1::Matrix3x2F &GetD2DMatrix() const;

	void Push(
		const DirectX::XMMATRIX &d3dMatrix,
		const D2D1_MATRIX_3X2_F &d2dMatrix);
	void Pop();

private:
	std::vector<DirectX::XMMATRIX, aligned_allocator<DirectX::XMMATRIX, 16>> d3dStack;
	std::vector<D2D1::Matrix3x2F> d2dStack;
};