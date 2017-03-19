#pragma once
#include "PlayerRenderer.h"
#include "..\Window\Window.h"
#include "..\Dx\HwndRenderer.h"

#include <memory>
#include <libhelpers\Thread\TaskQueueWorker.h>

class PlayerWindow : public Window {
public:
    PlayerWindow(WindowBaseData &baseData);
    virtual ~PlayerWindow();

    ProcessMsgResult ProcessMsg(uint32_t msg, WPARAM wparam, LPARAM lparam) override;

private:
    std::unique_ptr<HwndRenderer<PlayerRenderer>> renderer;
    TaskQueueWorker inputWorker;

    DirectX::XMFLOAT2 GetXMFLOAT2FromLParam(LPARAM lparam);
    DirectX::XMFLOAT2 GetPointFromLParam(LPARAM lparam);

    ProcessMsgResult ProcessInput(uint32_t msg, WPARAM wparam, LPARAM lparam);
};