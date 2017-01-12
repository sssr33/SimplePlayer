#pragma once
#include "libhelpers\Thread\LockStack.h"
#include "..\OMBlendState.h"

struct BlendState {
	Microsoft::WRL::ComPtr<ID3D11BlendState> state;
	float blendFactor[4];
	uint32_t sampleMask;

	BlendState(
		const Microsoft::WRL::ComPtr<ID3D11BlendState> &state,
		float blendFactor[4],
		uint32_t sampleMask);

	BlendState(
		const Microsoft::WRL::ComPtr<ID3D11BlendState> &state,
		const DirectX::XMVECTORF32 &blendFactor,
		uint32_t sampleMask);
};


class BlendStateCache {
public:
	BlendStateCache(ID3D11Device* dev);
	~BlendStateCache();

	std::shared_ptr<BlendState> GetDefaultState();
	std::shared_ptr<BlendState> GetAlphaBlendState();
	std::shared_ptr<BlendState> GetMultiplyBlendState();
	std::shared_ptr<BlendState> GetHighlightBlendState();
	std::shared_ptr<BlendState> GetAlphaBlendingBlendState();

private:
	ID3D11Device* dxDev;

	thread::critical_section cs;
	std::shared_ptr<BlendState> defaultState;
	std::shared_ptr<BlendState> alphaBlendState;
	std::shared_ptr<BlendState> multiplyBlendState;
	std::shared_ptr<BlendState> highlightingBlendState;
	std::shared_ptr<BlendState> alphaBlendingBlendState;
};
