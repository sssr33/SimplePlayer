#pragma once
#include "PlayerRenderer.h"
#include "..\Window\Window.h"
#include "..\Dx\HwndRenderer.h"

#include <memory>

class PlayerWindow : public Window {
public:
	PlayerWindow(WindowBaseData &baseData);
	virtual ~PlayerWindow();

	ProcessMsgResult ProcessMsg(uint32_t msg, WPARAM wparam, LPARAM lparam) override;

private:
	std::unique_ptr<HwndRenderer<PlayerRenderer>> renderer;
};