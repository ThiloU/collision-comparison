#include "openGJK_impl.h"
#include "support_mappings.h"

#define dotProduct(a, b) (a[0] * b[0] + a[1] * b[1] + a[2] * b[2])


void support_sphere(gkPolytope *body, const double *v) {
    printf("Support mapping for this type is not implemented");
}

void support_capsule(gkPolytope *body, const double *v) {
    printf("Support mapping for this type is not implemented");
}

void support_cylinder(gkPolytope *body, const double *v) {
    printf("Support mapping for this type is not implemented");
}

void support_box(gkPolytope *body, const double *v) {
    printf("Support mapping for this type is not implemented");
}

void support_mesh(gkPolytope *body, const double *v) {
    gkFloat s, maxs;
    gkFloat* vrt;
    int better = -1;

    maxs = dotProduct(body->s, v);

    for (int i = 0; i < body->numpoints; ++i) {
        vrt = body->coord[i];
        s = dotProduct(vrt, v);
        if (s > maxs) {
            maxs = s;
            better = i;
        }
    }

    if (better != -1) {
        body->s[0] = body->coord[better][0];
        body->s[1] = body->coord[better][1];
        body->s[2] = body->coord[better][2];
    }
}