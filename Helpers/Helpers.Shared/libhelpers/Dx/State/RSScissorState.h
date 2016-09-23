#pragma once
#include "libhelpers\Macros.h"

#include <array>

template<uint32_t Count>
class RSScissorState {
public:
	NO_COPY(RSScissorState);

	RSScissorState(ID3D11DeviceContext *d3dCtx)
		: d3dCtx(d3dCtx)
	{
		uint32_t tmpCount = Count;
		// TODO test save/restore with many rects
		this->d3dCtx->RSGetScissorRects(&tmpCount, this->scissorRects.data());
	}

	RSScissorState(RSScissorState &&other)
		: d3dCtx(std::move(other.d3dCtx)), scissorRects(std::move(other.scissorRects)) 
	{
		other.d3dCtx = nullptr;
	}

	~RSScissorState() {
		if (this->d3dCtx) {
			this->d3dCtx->RSSetScissorRects(Count, this->scissorRects.data());
		}
	}

	RSScissorState &operator=(RSScissorState &&other) {
		if (this != &other) {
			this->d3dCtx = std::move(other.d3dCtx);
			this->scissorRects = std::move(other.scissorRects);

			other.d3dCtx = nullptr;
		}

		return *this;
	}

private:
	ID3D11DeviceContext *d3dCtx;
	std::array<D3D11_RECT, Count> scissorRects;
};