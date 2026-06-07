
#pragma once

#include "collider.h"
#include "openGJK_impl.h"

using compare::Base::Collider;
using compare::Base::ColliderType;
using compare::Base::Case;

namespace compare::OpenGJK {
    struct OpenGJKCase {
        gkBody collider0;
        gkBody collider1;
    };

    void get_cases(const Case* base_cases, OpenGJKCase* openGJK_cases, int length);
    float get_distance(const OpenGJKCase& openGJK_case);
}


