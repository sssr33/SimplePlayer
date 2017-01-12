#pragma once
#include "..\DxDevice.h"

#include <libhelpers\Macros.h>

template<uint32_t ViewportCount>
class RSViewportState {
public:
	NO_COPY(RSViewportState);

	RSViewportState(ID3D11DeviceContext *d3dCtx)
		:d3dCtx(d3dCtx) 
	{
		auto tmpCnt = ViewportCount;
		this->d3dCtx->RSGetViewports(&tmpCnt, this->viewport.data());
	}

	RSViewportState(RSViewportState &&other)
		: d3dCtx(std::move(other.d3dCtx)),
		viewport(std::move(other.viewport))
	{
		other.d3dCtx = nullptr;
	}

	~RSViewportState() {
		if (this->d3dCtx) {
			this->d3dCtx->RSSetViewports(ViewportCount, this->viewport.data());
		}
	}

	RSViewportState &operator=(RSViewportState &&other) {
		if (this != &other) {
			this->d3dCtx = std::move(other.d3dCtx);
			this->viewport = std::move(other.viewport);
			
			other.d3dCtx = nullptr;
		}

		return *this;
	}

	const D3D11_VIEWPORT &GetViewport(size_t idx = 0) const {
		return viewport[idx];
	}

private:
	ID3D11DeviceContext *d3dCtx;

	std::array<D3D11_VIEWPORT, ViewportCount> viewport;
};

template<>
class RSViewportState<-1> {
public:
	NO_COPY(RSViewportState);

	RSViewportState(ID3D11DeviceContext *d3dCtx, uint32_t count)
		: d3dCtx(d3dCtx), viewport(count)
	{
		this->d3dCtx->RSGetViewports(&count, this->viewport.data());
	}

	RSViewportState(RSViewportState &&other)
		: d3dCtx(std::move(other.d3dCtx)),
		viewport(std::move(other.viewport))
	{
		other.d3dCtx = nullptr;
	}

	~RSViewportState() {
		if (this->d3dCtx) {
			this->d3dCtx->RSSetViewports((uint32_t)this->viewport.size(), this->viewport.data());
		}
	}

	RSViewportState &operator=(RSViewportState &&other) {
		if (this != &other) {
			this->d3dCtx = std::move(other.d3dCtx);
			this->viewport = std::move(other.viewport);

			other.d3dCtx = nullptr;
		}

		return *this;
	}

	const D3D11_VIEWPORT &GetViewport(size_t idx = 0) const {
		return viewport[idx];
	}

	uint32_t GetViewportCount() const {
		return (uint32_t)this->viewport.size();
	}

private:
	ID3D11DeviceContext *d3dCtx;
	std::vector<D3D11_VIEWPORT> viewport;
};