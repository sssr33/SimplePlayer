#include "BlendStateCache.h"
#include "libhelpers\HSystem.h"

BlendState::BlendState(
	const Microsoft::WRL::ComPtr<ID3D11BlendState>& state, 
	float blendFactor[4], uint32_t sampleMask) 
	: state(state), sampleMask(sampleMask) {
	this->blendFactor[0] = blendFactor[0];
	this->blendFactor[1] = blendFactor[1];
	this->blendFactor[2] = blendFactor[2];
	this->blendFactor[3] = blendFactor[3];
}

BlendState::BlendState(
	const Microsoft::WRL::ComPtr<ID3D11BlendState>& state, 
	const DirectX::XMVECTORF32 & blendFactor, uint32_t sampleMask)
	: state(state), sampleMask(sampleMask) {
	this->blendFactor[0] = blendFactor[0];
	this->blendFactor[1] = blendFactor[1];
	this->blendFactor[2] = blendFactor[2];
	this->blendFactor[3] = blendFactor[3];
}





BlendStateCache::BlendStateCache(ID3D11Device * dev)
	: dxDev(dev) {
}

BlendStateCache::~BlendStateCache() {
}

std::shared_ptr<BlendState> BlendStateCache::GetDefaultState() {
	thread::ThreadCsLock lk(this->cs);

	if (!this->defaultState) {
		HRESULT hr = S_OK;
		D3D11_BLEND_DESC blendDesc;
		D3D11_RENDER_TARGET_BLEND_DESC rtBlend;
		Microsoft::WRL::ComPtr<ID3D11BlendState> state;

		rtBlend.BlendEnable = FALSE;
		rtBlend.SrcBlend = D3D11_BLEND_ONE;
		rtBlend.DestBlend = D3D11_BLEND_ZERO;
		rtBlend.BlendOp = D3D11_BLEND_OP_ADD;
		rtBlend.SrcBlendAlpha = D3D11_BLEND_ONE;
		rtBlend.DestBlendAlpha = D3D11_BLEND_ZERO;
		rtBlend.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		rtBlend.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;

		for (auto &i : blendDesc.RenderTarget) {
			i = rtBlend;
		}

		hr = this->dxDev->CreateBlendState(&blendDesc, state.GetAddressOf());
		H::System::ThrowIfFailed(hr);

		this->defaultState = std::make_shared<BlendState>(state, DirectX::g_XMOne, UINT_MAX);
	}
	return this->defaultState;
}

std::shared_ptr<BlendState> BlendStateCache::GetAlphaBlendState() {
	thread::ThreadCsLock lk(this->cs);

	if (!this->alphaBlendState) {
		HRESULT hr = S_OK;
		D3D11_BLEND_DESC blendDesc;
		D3D11_RENDER_TARGET_BLEND_DESC rtBlend;
		Microsoft::WRL::ComPtr<ID3D11BlendState> state;

		rtBlend.BlendEnable = TRUE;
		rtBlend.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		rtBlend.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		rtBlend.BlendOp = D3D11_BLEND_OP_ADD;
		rtBlend.SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		rtBlend.DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		rtBlend.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		rtBlend.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;

		for (auto &i : blendDesc.RenderTarget) {
			i = rtBlend;
		}

		hr = this->dxDev->CreateBlendState(&blendDesc, state.GetAddressOf());
		H::System::ThrowIfFailed(hr);

		this->alphaBlendState = std::make_shared<BlendState>(state, DirectX::g_XMOne, UINT_MAX);
	}

	return this->alphaBlendState;
}

std::shared_ptr<BlendState> BlendStateCache::GetMultiplyBlendState() {
	thread::ThreadCsLock lk(this->cs);

	if (!this->multiplyBlendState) {
		HRESULT hr = S_OK;
		D3D11_BLEND_DESC blendDesc;
		D3D11_RENDER_TARGET_BLEND_DESC rtBlend;
		Microsoft::WRL::ComPtr<ID3D11BlendState> state;

		rtBlend.BlendEnable = TRUE;
		rtBlend.SrcBlend = D3D11_BLEND_DEST_COLOR;
		rtBlend.DestBlend = D3D11_BLEND_ZERO;
		rtBlend.BlendOp = D3D11_BLEND_OP_ADD;
		rtBlend.SrcBlendAlpha = D3D11_BLEND_ZERO;// D3D11_BLEND_ONE;
		rtBlend.DestBlendAlpha = D3D11_BLEND_ONE;// D3D11_BLEND_ZERO;
		rtBlend.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		rtBlend.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;

		for (auto &i : blendDesc.RenderTarget) {
			i = rtBlend;
		}

		hr = this->dxDev->CreateBlendState(&blendDesc, state.GetAddressOf());
		H::System::ThrowIfFailed(hr);

		this->multiplyBlendState = std::make_shared<BlendState>(state, DirectX::g_XMOne, UINT_MAX);
	}

	return this->multiplyBlendState;
}

std::shared_ptr<BlendState> BlendStateCache::GetHighlightBlendState() {
	thread::ThreadCsLock lk(this->cs);

	if (!this->highlightingBlendState) {
		HRESULT hr = S_OK;
		D3D11_BLEND_DESC blendDesc;
		D3D11_RENDER_TARGET_BLEND_DESC rtBlend;
		Microsoft::WRL::ComPtr<ID3D11BlendState> state;

		rtBlend.BlendEnable = TRUE;
		rtBlend.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		rtBlend.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		rtBlend.BlendOp = D3D11_BLEND_OP_ADD;
		rtBlend.SrcBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA;
		rtBlend.DestBlendAlpha = D3D11_BLEND_ONE;
		rtBlend.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		rtBlend.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;

		for (auto &i : blendDesc.RenderTarget) {
			i = rtBlend;
		}

		hr = this->dxDev->CreateBlendState(&blendDesc, state.GetAddressOf());
		H::System::ThrowIfFailed(hr);

		this->highlightingBlendState = std::make_shared<BlendState>(state, DirectX::g_XMOne, UINT_MAX);
	}

	return this->highlightingBlendState;
}

std::shared_ptr<BlendState> BlendStateCache::GetAlphaBlendingBlendState() {
	thread::ThreadCsLock lk(this->cs);

	if (!this->alphaBlendingBlendState) {
		HRESULT hr = S_OK;
		D3D11_BLEND_DESC blendDesc;
		D3D11_RENDER_TARGET_BLEND_DESC rtBlend;
		Microsoft::WRL::ComPtr<ID3D11BlendState> state;

		rtBlend.BlendEnable = TRUE;
		rtBlend.SrcBlend = D3D11_BLEND_ONE;
		rtBlend.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		rtBlend.BlendOp = D3D11_BLEND_OP_ADD;
		rtBlend.SrcBlendAlpha = D3D11_BLEND_ONE;
		rtBlend.DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		rtBlend.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		rtBlend.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;

		for (auto &i : blendDesc.RenderTarget) {
			i = rtBlend;
		}

		hr = this->dxDev->CreateBlendState(&blendDesc, state.GetAddressOf());
		H::System::ThrowIfFailed(hr);

		this->alphaBlendingBlendState = std::make_shared<BlendState>(state, DirectX::g_XMOne, UINT_MAX);
	}

	return this->alphaBlendingBlendState;
}
