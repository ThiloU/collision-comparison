#include "openGJK_impl.h"
#include "support_mappings.h"

#define dotProduct(a, b) (a[0] * b[0] + a[1] * b[1] + a[2] * b[2])

void transpose3x3(const gkFloat src[3][3], gkFloat dest[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            dest[j][i] = src[i][j];
        }
    }
}

void rotateVector3D(const gkFloat matrix[3][3], const gkFloat vec[3], gkFloat result[3]) {
    for (int i = 0; i < 3; i++) {
        result[i] = 0.0;
        for (int j = 0; j < 3; j++) {
            result[i] += matrix[i][j] * vec[j];
        }
    }
}

void support_sphere(gkBody *body, const gkFloat *v) {
    gkFloat vertex_local[3] = {0, 0, 0};
    const gkFloat v_length = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (v_length == 0.0){
        vertex_local[0] = 0;
        vertex_local[1] = 0;
        vertex_local[2] = body->radius;
    } else{
        vertex_local[0] = v[0] / v_length * body->radius;
        vertex_local[1] = v[1] / v_length * body->radius;
        vertex_local[2] = v[2] / v_length * body->radius;
    }

    body->s[0] = body->translation[0] + vertex_local[0];
    body->s[1] = body->translation[1] + vertex_local[1];
    body->s[2] = body->translation[2] + vertex_local[2];
}

void support_capsule(gkBody *body, const gkFloat *v) {
    gkFloat rotation_T[3][3];
    transpose3x3(body->rotation_mat, rotation_T);
    gkFloat local_dir[3];
    rotateVector3D(rotation_T, v, local_dir);

    const gkFloat v_length = sqrt(local_dir[0] * local_dir[0] + local_dir[1] * local_dir[1] + local_dir[2] * local_dir[2]);

    gkFloat vertex_local[3] = {0, 0, 0};
    if (v_length== 0.0){
        vertex_local[0] = body->radius;
    } else{
        vertex_local[0] = local_dir[0] / v_length * body->radius;
        vertex_local[1] = local_dir[1] / v_length * body->radius;
        vertex_local[2] = local_dir[2] / v_length * body->radius;
    }
    if (local_dir[2] > 0.0){
        vertex_local[2] += 0.5 * body->height;
    } else{
        vertex_local[2] -= 0.5 * body->height;
    }

    rotateVector3D(body->rotation_mat, vertex_local, body->s);
    body->s[0] += body->translation[0];
    body->s[1] += body->translation[1];
    body->s[2] += body->translation[2];
}

void support_cylinder(gkBody *body, const gkFloat *v) {
    gkFloat rotation_T[3][3];
    transpose3x3(body->rotation_mat, rotation_T);
    gkFloat local_dir[3];
    rotateVector3D(rotation_T, v, local_dir);

    gkFloat s = sqrt(local_dir[0] * local_dir[0] + local_dir[1] * local_dir[1]);

    gkFloat vertex_local[3] = {0, 0, 0};
    vertex_local[2] = body->height * (local_dir[2] < 0 ? -0.5 : 0.5);

    if (s == 0.0){
        vertex_local[0] = body->radius;
    } else{
        gkFloat d = body->radius / s;
        vertex_local[0] = local_dir[0] * d;
        vertex_local[1] = local_dir[1] * d;
    }

    rotateVector3D(body->rotation_mat, vertex_local, body->s);
    body->s[0] += body->translation[0];
    body->s[1] += body->translation[1];
    body->s[2] += body->translation[2];
}

void support_box(gkBody *body, const gkFloat *v) {
    gkFloat rotation_T[3][3];
    transpose3x3(body->rotation_mat, rotation_T);
    gkFloat local_dir[3];
    rotateVector3D(rotation_T, v, local_dir);

    gkFloat vertex_local[3] = {0, 0, 0};
    vertex_local[0] = local_dir[0] > 0 ? body->half_x_length : (local_dir[0] < 0 ? -body->half_x_length : 0);
    vertex_local[1] = local_dir[1] > 0 ? body->half_y_length : (local_dir[1] < 0 ? -body->half_y_length : 0);
    vertex_local[2] = local_dir[2] > 0 ? body->half_z_length : (local_dir[2] < 0 ? -body->half_z_length : 0);

    rotateVector3D(body->rotation_mat, vertex_local, body->s);
    body->s[0] += body->translation[0];
    body->s[1] += body->translation[1];
    body->s[2] += body->translation[2];
}

void support_mesh(gkBody *body, const gkFloat *v) {
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