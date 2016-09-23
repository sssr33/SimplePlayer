#include "DxLayerStackState.h"

DxLayerStackState::DxLayerStackState(
	RSState &&resourcesState,
	RSScissorState<1> &&scissorState)
	: resourcesState(std::move(resourcesState)), 
	scissorState(std::move(scissorState)) 
{
}

DxLayerStackState::DxLayerStackState(DxLayerStackState &&other)
	: resourcesState(std::move(other.resourcesState)), 
	scissorState(std::move(other.scissorState)) 
{
}

DxLayerStackState &DxLayerStackState::operator=(DxLayerStackState &&other) {
	if (this != &other) {
		this->resourcesState = std::move(other.resourcesState);
		this->scissorState = std::move(other.scissorState);
	}

	return *this;
}