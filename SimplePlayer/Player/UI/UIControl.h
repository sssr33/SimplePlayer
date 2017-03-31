#pragma once

namespace UI {
    class Control {
    public:
        Control();
        virtual ~Control();

        virtual float GetX() = 0;
        virtual void SetX(float v) = 0;

        virtual float GetY() = 0;
        virtual void SetY(float v) = 0;

        virtual float GetWidth() = 0;
        virtual void SetWidth(float v) = 0;

        virtual float GetHeight() = 0;
        virtual void SetHeight(float v) = 0;

        virtual Control *GetParent() = 0;

    protected:
        void SetParentTo(Control *v);
        virtual void SetParent(Control *v) = 0;
    };
}