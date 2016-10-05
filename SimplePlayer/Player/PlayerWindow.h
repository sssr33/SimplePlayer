#pragma once
#include "..\Window\Window.h"

class PlayerWindow : public Window {
public:
	PlayerWindow(WindowBaseData &baseData);
	virtual ~PlayerWindow();

	ProcessMsgResult ProcessMsg(uint32_t msg, WPARAM wparam, LPARAM lparam) override;

private:

};