#pragma once
#include "OMRenderTargetState.h"
#include "RSViewportState.h"

template<uint32_t RtCount>
class D3DTargetState {
public:
	NO_COPY(D3DTargetState);

	D3DTargetState(ID3D11DeviceContext *d3dCtx)
		: omRtState(d3dCtx), rsViewportState(d3dCtx) {
	}

	D3DTargetState(D3DTargetState &&other)
		: omRtState(std::move(other.omRtState)),
		rsViewportState(std::move(other.rsViewportState))
	{
	}

	D3DTargetState &operator=(D3DTargetState &&other) {
		if (this != &other) {
			this->omRtState = std::move(other.omRtState);
			this->rsViewportState = std::move(other.rsViewportState);
		}

		return *this;
	}

private:
	OMRenderTargetState<RtCount> omRtState;
	RSViewportState<RtCount> rsViewportState;
};