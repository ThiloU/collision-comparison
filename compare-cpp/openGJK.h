
#pragma once

#include "collider.h"
#include "openGJK_impl.h"
#include <vector>

using compare::Base::Collider;
using compare::Base::ColliderType;
using compare::Base::Case;

namespace compare::OpenGJK {
    struct MeshAdjacency {
        std::vector<unsigned int> neighbor_offsets;
        std::vector<unsigned int> neighbors;
    };

    struct OpenGJKCollider {
        gkBody collider;
        MeshAdjacency adjacency;
        mutable unsigned int last_support_vertex = 0;
    };

    struct OpenGJKCase {
        OpenGJKCollider collider0;
        OpenGJKCollider collider1;
    };

    void get_cases(const Case* base_cases, OpenGJKCase* openGJK_cases, int length);
    float get_distance(const OpenGJKCase& openGJK_case);
}


