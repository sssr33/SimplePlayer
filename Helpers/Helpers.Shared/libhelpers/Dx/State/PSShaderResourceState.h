#pragma once
#include "..\DxDevice.h"

#include <libhelpers\Macros.h>
#include <libhelpers\Containers\comptr_array.h>
#include <libhelpers\Containers\comptr_vector.h>

template<uint32_t Count>
class PSShaderResourceState {
public:
	NO_COPY(PSShaderResourceState);

	PSShaderResourceState(ID3D11DeviceContext *d3dCtx, uint32_t startSlot = 0)
		: d3dCtx(d3dCtx), startSlot(startSlot)
	{
		this->d3dCtx->PSGetShaderResources(this->startSlot, (uint32_t)this->srv.size(), this->srv.data());
	}

	PSShaderResourceState(PSShaderResourceState &&other)
		: d3dCtx(std::move(other.d3dCtx)),
		startSlot(std::move(other.startSlot)),
		srv(std::move(other.srv))
	{
		other.d3dCtx = nullptr;
	}

	~PSShaderResourceState() {
		if (this->d3dCtx) {
			this->d3dCtx->PSSetShaderResources(this->startSlot, (uint32_t)this->srv.size(), this->srv.data());
		}
	}

	PSShaderResourceState &operator=(PSShaderResourceState &&other) {
		if (this != &other) {
			this->d3dCtx = std::move(other.d3dCtx);
			this->startSlot = std::move(other.startSlot);
			this->srv = std::move(other.srv);

			other.d3dCtx = nullptr;
		}

		return *this;
	}

private:
	uint32_t startSlot;
	ID3D11DeviceContext *d3dCtx;
	comptr_array<ID3D11ShaderResourceView, Count> srv;
};

template<>
class PSShaderResourceState<-1> {
public:
	NO_COPY(PSShaderResourceState);

	PSShaderResourceState(ID3D11DeviceContext *d3dCtx, uint32_t count, uint32_t startSlot = 0)
		: d3dCtx(d3dCtx), startSlot(startSlot)
	{
		this->srv.resize(count);
		this->d3dCtx->PSGetShaderResources(this->startSlot, (uint32_t)this->srv.size(), this->srv.data());
	}

	PSShaderResourceState(PSShaderResourceState &&other)
		: d3dCtx(std::move(other.d3dCtx)),
		startSlot(std::move(other.startSlot)),
		srv(std::move(other.srv))
	{
		other.d3dCtx = nullptr;
	}

	~PSShaderResourceState() {
		if (this->d3dCtx) {
			this->d3dCtx->PSSetShaderResources(this->startSlot, (uint32_t)this->srv.size(), this->srv.data());
		}
	}

	PSShaderResourceState &operator=(PSShaderResourceState &&other) {
		if (this != &other) {
			this->d3dCtx = std::move(other.d3dCtx);
			this->startSlot = std::move(other.startSlot);
			this->srv = std::move(other.srv);

			other.d3dCtx = nullptr;
		}

		return *this;
	}

private:
	uint32_t startSlot;
	ID3D11DeviceContext *d3dCtx;
	comptr_vector<ID3D11ShaderResourceView> srv;
};