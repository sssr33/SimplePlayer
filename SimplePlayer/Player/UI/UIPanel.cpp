#include "UIPanel.h"

namespace UI {
    Panel::Panel() 
        : child(nullptr)
    {}

    Panel::~Panel() {
    }

    Control *Panel::GetChild() const {
        return this->child;
    }

    void Panel::SetChild(Control *v) {
        this->child = v;
        this->SetParentTo(v);
    }
}