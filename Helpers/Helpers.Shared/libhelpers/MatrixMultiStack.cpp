#include "MatrixMultiStack.h"
#include "HMath.h"

MatrixMultiStack::PushScope::PushScope()
	: parent(nullptr)
{
}

MatrixMultiStack::PushScope::PushScope(MatrixMultiStack *parent, size_t stackIdx)
	: parent(parent), stackIdx(stackIdx)
{
}

MatrixMultiStack::PushScope::PushScope(PushScope &&other)
	: parent(std::move(other.parent)),
	stackIdx(std::move(other.stackIdx))
{
	other.parent = nullptr;
}

MatrixMultiStack::PushScope::~PushScope() {
	if (parent) {
		parent->Pop(this->stackIdx);
	}
}

MatrixMultiStack::PushScope &MatrixMultiStack::PushScope::operator=(PushScope &&other) {
	if (this != &other) {
		this->parent = other.parent;
		this->stackIdx = other.stackIdx;

		other.parent = nullptr;
	}

	return *this;
}

const DirectX::XMMATRIX &MatrixMultiStack::GetD3DMatrix() const {
	this->UpdateMatrixCache();

	auto &d3dMatrix = *this->cache.d3dMatrix;
	return d3dMatrix;
}

const D2D1::Matrix3x2F &MatrixMultiStack::GetD2DMatrix() const {
	this->UpdateMatrixCache();

	auto &d2dMatrix = this->cache.d2dMatrix;
	return d2dMatrix;
}

void MatrixMultiStack::Resize(size_t size) {
	this->stacks.resize(size);
}

MatrixMultiStack::PushScope MatrixMultiStack::PushCumulativeScoped(
	size_t idx,
	const DirectX::XMMATRIX &d3dMatrix,
	const D2D1_MATRIX_3X2_F &d2dMatrix)
{
	this->PushCumulative(idx, d3dMatrix, d2dMatrix);
	return MatrixMultiStack::PushScope(this, idx);
}

MatrixMultiStack::PushScope MatrixMultiStack::PushCumulativeScoped(
	size_t idx,
	const DirectX::XMMATRIX &d3dMatrix)
{
	this->PushCumulative(idx, d3dMatrix);
	return MatrixMultiStack::PushScope(this, idx);
}

MatrixMultiStack::PushScope MatrixMultiStack::PushCumulativeScoped(
	size_t idx,
	const D2D1_MATRIX_3X2_F &d2dMatrix)
{
	this->PushCumulative(idx, d2dMatrix);
	return MatrixMultiStack::PushScope(this, idx);
}

MatrixMultiStack::PushScope MatrixMultiStack::PushScoped(
	size_t idx,
	const DirectX::XMMATRIX &d3dMatrix,
	const D2D1_MATRIX_3X2_F &d2dMatrix)
{
	this->Push(idx, d3dMatrix, d2dMatrix);
	return MatrixMultiStack::PushScope(this, idx);
}

MatrixMultiStack::PushScope MatrixMultiStack::PushScoped(
	size_t idx,
	const DirectX::XMMATRIX &d3dMatrix)
{
	this->Push(idx, d3dMatrix);
	return MatrixMultiStack::PushScope(this, idx);
}

MatrixMultiStack::PushScope MatrixMultiStack::PushScoped(
	size_t idx,
	const D2D1_MATRIX_3X2_F &d2dMatrix)
{
	this->Push(idx, d2dMatrix);
	return MatrixMultiStack::PushScope(this, idx);
}

void MatrixMultiStack::PushCumulative(
	size_t idx,
	const DirectX::XMMATRIX &d3dMatrix,
	const D2D1_MATRIX_3X2_F &d2dMatrix)
{
	auto &stack = this->stacks[idx];

	if (!stack.IsEmpty()) {
		DirectX::XMMATRIX d3dMatrixTmp;
		D2D1_MATRIX_3X2_F d2dMatrixTmp;

		d3dMatrixTmp = DirectX::XMMatrixMultiply(d3dMatrix, stack.GetD3DMatrix());
		d2dMatrixTmp = d2dMatrix * stack.GetD2DMatrix();

		this->Push(idx, d3dMatrixTmp, d2dMatrixTmp);
	}
	else {
		this->Push(idx, d3dMatrix, d2dMatrix);
	}
}

void MatrixMultiStack::PushCumulative(
	size_t idx,
	const DirectX::XMMATRIX &d3dMatrix)
{
	this->PushCumulative(idx, d3dMatrix, H::Math::D2D1Matrix3X2From(d3dMatrix));
}

void MatrixMultiStack::PushCumulative(
	size_t idx,
	const D2D1_MATRIX_3X2_F &d2dMatrix)
{
	this->PushCumulative(idx, H::Math::XMMatrixFrom(d2dMatrix), d2dMatrix);
}

void MatrixMultiStack::Push(
	size_t idx,
	const DirectX::XMMATRIX &d3dMatrix,
	const D2D1_MATRIX_3X2_F &d2dMatrix)
{
	auto &stack = this->stacks[idx];

	stack.Push(d3dMatrix, d2dMatrix);
	this->cache.matrixDirty = true;
}

void MatrixMultiStack::Push(
	size_t idx,
	const DirectX::XMMATRIX &d3dMatrix)
{
	this->Push(idx, d3dMatrix, H::Math::D2D1Matrix3X2From(d3dMatrix));
}

void MatrixMultiStack::Push(
	size_t idx,
	const D2D1_MATRIX_3X2_F &d2dMatrix)
{
	this->Push(idx, H::Math::XMMatrixFrom(d2dMatrix), d2dMatrix);
}

void MatrixMultiStack::Pop(size_t idx) {
	auto &stack = this->stacks[idx];

	stack.Pop();
	this->cache.matrixDirty = true;
}

MatrixMultiStack::Cache::Cache()
	: d3dMatrix(DirectX::XMMatrixIdentity()), d2dMatrix(D2D1::Matrix3x2F::Identity()),
	matrixDirty(false) {
}

void MatrixMultiStack::UpdateMatrixCache() const {
	if (this->cache.matrixDirty) {
		this->cache.matrixDirty = false;

		if (this->stacks.empty()) {
			this->cache.d3dMatrix = DirectX::XMMatrixIdentity();
			this->cache.d2dMatrix = D2D1::Matrix3x2F::Identity();
		}
		else {
			auto it = this->stacks.begin();

			this->cache.d3dMatrix = it->GetD3DMatrix();
			this->cache.d2dMatrix = it->GetD2DMatrix();

			++it;

			for (; it != this->stacks.end(); ++it) {
				if (!it->IsEmpty()) {
					this->cache.d3dMatrix = DirectX::XMMatrixMultiply(*this->cache.d3dMatrix, it->GetD3DMatrix());
					this->cache.d2dMatrix = this->cache.d2dMatrix * it->GetD2DMatrix();
				}
			}
		}
	}
}