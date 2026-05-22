
#ifndef COLLISION_COMPARISON_SUPPORT_MAPPINGS_H
#define COLLISION_COMPARISON_SUPPORT_MAPPINGS_H

#endif //COLLISION_COMPARISON_SUPPORT_MAPPINGS_H
#include "openGJK_impl.h"

void support_sphere(gkPolytope *body, const double *v);

void support_capsule(gkPolytope *body, const double *v);

void support_cylinder(gkPolytope *body, const double *v);

void support_box(gkPolytope *body, const double *v);

void support_mesh(gkPolytope *body, const double *v);
