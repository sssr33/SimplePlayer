#pragma once
#include "UIControlSimple.h"

namespace UI {
    class Panel : public ControlSimple {
    public:
        Panel();
        virtual ~Panel();

        Control *GetChild() const;
        void SetChild(Control *v);

    private:
        Control *child;
    };
}