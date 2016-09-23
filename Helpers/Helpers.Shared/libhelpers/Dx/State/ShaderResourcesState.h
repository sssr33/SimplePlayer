#pragma once
#include "libhelpers\Macros.h"
#include "libhelpers\Containers\comptr_array.h"
#include "libhelpers\Dx\DxIncludes.h"
#include "libhelpers\Dx\ShaderType.h"

#include <array>

template<ShaderType> struct ShaderResourceAccessor {
	static void Get(ID3D11DeviceContext *d3dCtx, uint32_t startSlot, uint32_t count, ID3D11ShaderResourceView **resources) {
		static_assert(false, "Not implemented.");
	}

	static void Set(ID3D11DeviceContext *d3dCtx, uint32_t startSlot, uint32_t count, ID3D11ShaderResourceView *const *resources) {
		static_assert(false, "Not implemented.");
	}
};

template<> struct ShaderResourceAccessor<ShaderType::VS> {
	static void Get(ID3D11DeviceContext *d3dCtx, uint32_t startSlot, uint32_t count, ID3D11ShaderResourceView **resources) {
		d3dCtx->VSGetShaderResources(startSlot, count, resources);
	}

	static void Set(ID3D11DeviceContext *d3dCtx, uint32_t startSlot, uint32_t count, ID3D11ShaderResourceView *const *resources) {
		d3dCtx->VSSetShaderResources(startSlot, count, resources);
	}
};

template<> struct ShaderResourceAccessor<ShaderType::HS> {
	static void Get(ID3D11DeviceContext *d3dCtx, uint32_t startSlot, uint32_t count, ID3D11ShaderResourceView **resources) {
		d3dCtx->HSGetShaderResources(startSlot, count, resources);
	}

	static void Set(ID3D11DeviceContext *d3dCtx, uint32_t startSlot, uint32_t count, ID3D11ShaderResourceView *const *resources) {
		d3dCtx->HSSetShaderResources(startSlot, count, resources);
	}
};

template<> struct ShaderResourceAccessor<ShaderType::DS> {
	static void Get(ID3D11DeviceContext *d3dCtx, uint32_t startSlot, uint32_t count, ID3D11ShaderResourceView **resources) {
		d3dCtx->DSGetShaderResources(startSlot, count, resources);
	}

	static void Set(ID3D11DeviceContext *d3dCtx, uint32_t startSlot, uint32_t count, ID3D11ShaderResourceView *const *resources) {
		d3dCtx->DSSetShaderResources(startSlot, count, resources);
	}
};

template<> struct ShaderResourceAccessor<ShaderType::GS> {
	static void Get(ID3D11DeviceContext *d3dCtx, uint32_t startSlot, uint32_t count, ID3D11ShaderResourceView **resources) {
		d3dCtx->GSGetShaderResources(startSlot, count, resources);
	}

	static void Set(ID3D11DeviceContext *d3dCtx, uint32_t startSlot, uint32_t count, ID3D11ShaderResourceView *const *resources) {
		d3dCtx->GSSetShaderResources(startSlot, count, resources);
	}
};

template<> struct ShaderResourceAccessor<ShaderType::PS> {
	static void Get(ID3D11DeviceContext *d3dCtx, uint32_t startSlot, uint32_t count, ID3D11ShaderResourceView **resources) {
		d3dCtx->PSGetShaderResources(startSlot, count, resources);
	}

	static void Set(ID3D11DeviceContext *d3dCtx, uint32_t startSlot, uint32_t count, ID3D11ShaderResourceView *const *resources) {
		d3dCtx->PSSetShaderResources(startSlot, count, resources);
	}
};

template<uint32_t Count, ShaderType Type>
class ShaderResourcesState {
public:
	NO_COPY(ShaderResourcesState);

	ShaderResourcesState(ID3D11DeviceContext *d3dCtx, uint32_t startSlot = 0)
		: d3dCtx(d3dCtx), startSlot(startSlot)
	{
		ShaderResourceAccessor<Type>::Get(this->d3dCtx, startSlot, Count, this->resources.data());
	}

	ShaderResourcesState(ShaderResourcesState &&other)
		: d3dCtx(std::move(other.d3dCtx)), 
		resources(std::move(other.resources)),
		startSlot(std::move(other.startSlot))
	{
		other.d3dCtx = nullptr;
	}

	~ShaderResourcesState() {
		if (this->d3dCtx) {
			ShaderResourceAccessor<Type>::Set(this->d3dCtx, startSlot, Count, this->resources.data());
		}
	}

	ShaderResourcesState &operator=(ShaderResourcesState &&other) {
		if (this != &other) {
			this->d3dCtx = std::move(other.d3dCtx);
			this->resources = std::move(other.resources);
			this->startSlot = std::move(other.startSlot);

			other.d3dCtx = nullptr;
		}

		return *this;
	}

private:
	uint32_t startSlot;
	ID3D11DeviceContext *d3dCtx;
	comptr_array<ID3D11ShaderResourceView, Count> resources;
};