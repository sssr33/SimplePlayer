#pragma once
#include "DxLayerStackResources.h"
#include "libhelpers\Dx\State\RSScissorState.h"
#include "libhelpers\Macros.h"

class DxLayerStackState {
public:
	NO_COPY(DxLayerStackState);

	DxLayerStackState(
		RSState &&resourcesState, 
		RSScissorState<1> &&scissorState);
	DxLayerStackState(DxLayerStackState &&other);

	DxLayerStackState &operator=(DxLayerStackState &&other);

private:
	RSState resourcesState;
	RSScissorState<1> scissorState;
};