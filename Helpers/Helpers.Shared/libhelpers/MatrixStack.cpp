#include "MatrixStack.h"
#include "HMath.h"
#include "HSystem.h"

bool MatrixStack::IsEmpty() const {
	H::System::Assert(this->d3dStack.empty() == this->d2dStack.empty());
	bool empty = this->d3dStack.empty();
	return empty;
}

const DirectX::XMMATRIX &MatrixStack::GetD3DMatrix() const {
	if (this->d3dStack.empty()) {
		return H::Math::Identity<DirectX::XMMATRIX>();
	}
	else {
		return this->d3dStack.back();
	}
}

const D2D1::Matrix3x2F &MatrixStack::GetD2DMatrix() const {
	if (this->d2dStack.empty()) {
		return H::Math::Identity<D2D1::Matrix3x2F>();
	}
	else {
		return this->d2dStack.back();
	}
}

void MatrixStack::Push(
	const DirectX::XMMATRIX &d3dMatrix,
	const D2D1_MATRIX_3X2_F &d2dMatrix)
{
	auto d2dMatrixTmp = D2D1::Matrix3x2F::ReinterpretBaseType(&d2dMatrix);

	this->d3dStack.push_back(d3dMatrix);
	this->d2dStack.push_back(*d2dMatrixTmp);
}

void MatrixStack::Pop() {
	this->d3dStack.pop_back();
	this->d2dStack.pop_back();
}