#include "UIControlSimple.h"

namespace UI {
    ControlSimple::ControlSimple() 
        : x(0.0f), y(0.0f), width(0.0f), height(0.0f),
        parent(nullptr)
    {}

    ControlSimple::~ControlSimple() {
    }

    float ControlSimple::GetX() {
        return this->x;
    }

    void ControlSimple::SetX(float v) {
        this->x = v;
    }

    float ControlSimple::GetY() {
        return this->y;
    }

    void ControlSimple::SetY(float v) {
        this->y = v;
    }

    float ControlSimple::GetWidth() {
        return this->width;
    }

    void ControlSimple::SetWidth(float v) {
        this->width = v;
    }

    float ControlSimple::GetHeight() {
        return this->height;
    }

    void ControlSimple::SetHeight(float v) {
        this->height = v;
    }

    Control *ControlSimple::GetParent() {
        return this->parent;
    }

    void ControlSimple::SetParent(Control *v) {
        this->parent = v;
    }
}