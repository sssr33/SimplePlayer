#pragma once
#include "..\Window\Window.h"

class PlayerWindow : public Window {
public:
	PlayerWindow(WindowBaseData &baseData);
	virtual ~PlayerWindow();

	void ProcessMsg(uint32_t msg, WPARAM wparam, LPARAM lparam) override;

private:

};