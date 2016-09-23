#pragma once
#include "OMRenderTargetState.h"
#include "RSViewportState.h"

template<uint32_t RtCount>
class RenderTargetState {
public:
	NO_COPY(RenderTargetState);

	RenderTargetState(ID3D11DeviceContext *d3dCtx)
		: omRtState(d3dCtx), rsViewportState(d3dCtx) {
	}

	RenderTargetState(RenderTargetState &&other)
		: omRtState(std::move(other.omRtState)),
		rsViewportState(std::move(other.rsViewportState))
	{
	}

	RenderTargetState &operator=(RenderTargetState &&other) {
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