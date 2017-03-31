#pragma once
#include "UIControl.h"

namespace UI {
    class ControlSimple : public Control {
    public:
        ControlSimple();
        virtual ~ControlSimple();

        float GetX() override;
        void SetX(float v) override;

        float GetY() override;
        void SetY(float v) override;

        float GetWidth() override;
        void SetWidth(float v) override;

        float GetHeight() override;
        void SetHeight(float v) override;

        Control *GetParent() override;
        void SetParent(Control *v) override;

    private:
        Control *parent;
        float x, y, width, height;
    };
}