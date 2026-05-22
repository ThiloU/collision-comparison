
#pragma once

#include "collider.h"
#include "BulletCollision/CollisionShapes/btSphereShape.h"
#include "BulletCollision/CollisionShapes/btCapsuleShape.h"
#include "BulletCollision/CollisionShapes/btCylinderShape.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"

using compare::Base::Collider;
using compare::Base::ColliderType;
using compare::Base::Case;

namespace compare::OpenGJK {
    struct OpenGJKCollider {
        btSphereShape sphere;
        btCapsuleShapeX capsule;
        btCylinderShape cylinder;
        btBoxShape box;

        btConvexShape* shape;
    };

    struct OpenGJKCase {
        btTransform transform0;
        btTransform transform1;

        OpenGJKCollider collider0;
        OpenGJKCollider collider1;
    };

    void get_cases(Case* base_cases, OpenGJKCase* openGJK_cases, int length);
    float get_distance(OpenGJKCase& openGJK_case);
}


