#pragma once
#include "MatrixStack.h"
#include "Aligned.h"
#include "ScopedPushBase.h"
#include "Macros.h"

#include <d2d1.h>
#include <DirectXMath.h>
#include <vector>

class MatrixMultiStack {
public:
	class PushScope {
	public:
		NO_COPY(PushScope);

		PushScope();
		PushScope(MatrixMultiStack *parent, size_t stackIdx);
		PushScope(PushScope &&other);
		~PushScope();

		PushScope &operator=(PushScope &&other);

	private:
		MatrixMultiStack *parent;
		size_t stackIdx;
	};

	const DirectX::XMMATRIX &GetD3DMatrix() const;
	const D2D1::Matrix3x2F &GetD2DMatrix() const;

	void Resize(size_t size);

	PushScope PushCumulativeScoped(
		size_t idx,
		const DirectX::XMMATRIX &d3dMatrix,
		const D2D1_MATRIX_3X2_F &d2dMatrix);
	PushScope PushCumulativeScoped(
		size_t idx,
		const DirectX::XMMATRIX &d3dMatrix);
	PushScope PushCumulativeScoped(
		size_t idx,
		const D2D1_MATRIX_3X2_F &d2dMatrix);

	PushScope PushScoped(
		size_t idx,
		const DirectX::XMMATRIX &d3dMatrix,
		const D2D1_MATRIX_3X2_F &d2dMatrix);
	PushScope PushScoped(
		size_t idx,
		const DirectX::XMMATRIX &d3dMatrix);
	PushScope PushScoped(
		size_t idx,
		const D2D1_MATRIX_3X2_F &d2dMatrix);

	void PushCumulative(
		size_t idx,
		const DirectX::XMMATRIX &d3dMatrix,
		const D2D1_MATRIX_3X2_F &d2dMatrix);
	void PushCumulative(
		size_t idx,
		const DirectX::XMMATRIX &d3dMatrix);
	void PushCumulative(
		size_t idx,
		const D2D1_MATRIX_3X2_F &d2dMatrix);

	void Push(
		size_t idx,
		const DirectX::XMMATRIX &d3dMatrix,
		const D2D1_MATRIX_3X2_F &d2dMatrix);
	void Push(
		size_t idx,
		const DirectX::XMMATRIX &d3dMatrix);
	void Push(
		size_t idx,
		const D2D1_MATRIX_3X2_F &d2dMatrix);

	void Pop(size_t idx);

	// helpers for using methods with strongly typed enums
	template<class T>
	void Resize(T size) {
		this->Resize(static_cast<size_t>(size));
	}

	template<class T>
	PushScope PushCumulativeScoped(
		T idx,
		const DirectX::XMMATRIX &d3dMatrix,
		const D2D1_MATRIX_3X2_F &d2dMatrix)
	{
		return this->PushCumulativeScoped(static_cast<size_t>(idx), d3dMatrix, d2dMatrix);
	}

	template<class T>
	PushScope PushCumulativeScoped(
		T idx,
		const DirectX::XMMATRIX &d3dMatrix)
	{
		return this->PushCumulativeScoped(static_cast<size_t>(idx), d3dMatrix);
	}

	template<class T>
	PushScope PushCumulativeScoped(
		T idx,
		const D2D1_MATRIX_3X2_F &d2dMatrix)
	{
		return this->PushCumulativeScoped(static_cast<size_t>(idx), d2dMatrix);
	}

	template<class T>
	PushScope PushScoped(
		T idx,
		const DirectX::XMMATRIX &d3dMatrix,
		const D2D1_MATRIX_3X2_F &d2dMatrix)
	{
		return this->PushScoped(static_cast<size_t>(idx), d3dMatrix, d2dMatrix);
	}

	template<class T>
	PushScope PushScoped(
		T idx,
		const DirectX::XMMATRIX &d3dMatrix)
	{
		return this->PushScoped(static_cast<size_t>(idx), d3dMatrix);
	}

	template<class T>
	PushScope PushScoped(
		T idx,
		const D2D1_MATRIX_3X2_F &d2dMatrix)
	{
		return this->PushScoped(static_cast<size_t>(idx), d2dMatrix);
	}

	template<class T>
	void PushCumulative(
		T idx,
		const DirectX::XMMATRIX &d3dMatrix,
		const D2D1_MATRIX_3X2_F &d2dMatrix)
	{
		return this->PushCumulative(static_cast<size_t>(idx), d3dMatrix, d2dMatrix);
	}

	template<class T>
	void PushCumulative(
		T idx,
		const DirectX::XMMATRIX &d3dMatrix)
	{
		return this->PushCumulative(static_cast<size_t>(idx), d3dMatrix);
	}

	template<class T>
	void PushCumulative(
		T idx,
		const D2D1_MATRIX_3X2_F &d2dMatrix)
	{
		return this->PushCumulative(static_cast<size_t>(idx), d2dMatrix);
	}

	template<class T>
	void Push(
		T idx,
		const DirectX::XMMATRIX &d3dMatrix,
		const D2D1_MATRIX_3X2_F &d2dMatrix)
	{
		return this->Push(static_cast<size_t>(idx), d3dMatrix, d2dMatrix);
	}

	template<class T>
	void Push(
		T idx,
		const DirectX::XMMATRIX &d3dMatrix)
	{
		return this->Push(static_cast<size_t>(idx), d3dMatrix);
	}

	template<class T>
	void Push(
		T idx,
		const D2D1_MATRIX_3X2_F &d2dMatrix)
	{
		return this->Push(static_cast<size_t>(idx), d2dMatrix);
	}

	template<class T>
	void Pop(T idx) {
		this->Pop(static_cast<size_t>(idx));
	}

private:
	struct Cache {
		Aligned<DirectX::XMMATRIX, 16> d3dMatrix;
		D2D1::Matrix3x2F d2dMatrix;

		bool matrixDirty;

		Cache();
	};

	mutable Cache cache;
	std::vector<MatrixStack> stacks;

	void UpdateMatrixCache() const;
};