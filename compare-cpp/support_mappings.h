
#ifndef COLLISION_COMPARISON_SUPPORT_MAPPINGS_H
#define COLLISION_COMPARISON_SUPPORT_MAPPINGS_H

#endif //COLLISION_COMPARISON_SUPPORT_MAPPINGS_H
#include "openGJK_impl.h"

void support_sphere(gkBody *body, const gkFloat *v);

void support_capsule(gkBody *body, const gkFloat *v);

void support_cylinder(gkBody *body, const gkFloat *v);

void support_box(gkBody *body, const gkFloat *v);

void support_mesh(gkBody *body, const gkFloat *v);
