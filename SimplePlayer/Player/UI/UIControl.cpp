#include "UIControl.h"

namespace UI {
    Control::Control() {}

    Control::~Control() {}

    void Control::SetParentTo(Control *v) {
        v->SetParent(this);
    }
}