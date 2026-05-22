#include "openGJK.h"

#include <iostream>


#include "openGJK_impl.h"


#include <math.h>
#define fscanf_s fscanf

namespace compare::OpenGJK {
    btTransform get_transform(Collider collider){
        return btTransform(
                btMatrix3x3(
                        btVector3(collider.colliderToOrigen[0][1], collider.colliderToOrigen[1][0], collider.colliderToOrigen[2][0]),
                        btVector3(collider.colliderToOrigen[0][2], collider.colliderToOrigen[1][1], collider.colliderToOrigen[2][1]),
                        btVector3(collider.colliderToOrigen[0][2], collider.colliderToOrigen[1][2], collider.colliderToOrigen[2][2])),
                btVector3(collider.colliderToOrigen[0][3], collider.colliderToOrigen[1][3], collider.colliderToOrigen[2][3]));
    }

    void get_collider(Collider collider, OpenGJKCollider& openGJK_collider){
        if (collider.type == ColliderType::Sphere){
            openGJK_collider.sphere = btSphereShape(get_radius(collider));
            openGJK_collider.shape = &openGJK_collider.sphere;
        }

        if (collider.type == ColliderType::Cylinder){
            openGJK_collider.cylinder = btCylinderShapeX(btVector3(get_radius(collider), 0.0 , get_height(collider) / 2));
            openGJK_collider.shape = &openGJK_collider.cylinder;
        }

        if (collider.type == ColliderType::Capsule){
            openGJK_collider.capsule = btCapsuleShapeX(get_radius(collider), get_height(collider));
            openGJK_collider.shape = &openGJK_collider.capsule;
        }

        if (collider.type == ColliderType::Box){
            openGJK_collider.box = btBoxShape(btVector3(get_size_x(collider) / 2, get_size_y(collider) / 2, get_size_z(collider) / 2));
            openGJK_collider.shape = &openGJK_collider.box;
        }
    }

    void get_case(Collider collider0, Collider collider1, OpenGJKCase& openGJK_case){
        openGJK_case.transform0 = get_transform(collider0);
        openGJK_case.transform1 = get_transform(collider1);

        get_collider(collider0, openGJK_case.collider0);
        get_collider(collider1, openGJK_case.collider1);
    }

    void get_cases(Case* base_cases, OpenGJKCase* openGJK_cases, int length){

        for (int i = 0; i < length; i++) {

            auto base_case = base_cases[i];
            get_case(base_case.collider0, base_case.collider1, openGJK_cases[i]);

        }
    }

    // int readinput(const char* inputfile, gkFloat*** pts, int* out) {
    //     int npoints = 0;
    //     int idx = 0;
    //     FILE* fp;
    //
    //     /* Open file. */
    //     if ((fp = fopen(inputfile, "r")) == NULL) {
    //         fprintf(stdout, "ERROR: input file %s not found!\n", inputfile);
    //         fprintf(stdout, "  -> The file must be in the folder from which this "
    //                         "program is launched\n\n");
    //         return 1;
    //     }
    //
    //     /* Read number of input vertices. */
    //     if (fscanf_s(fp, "%d", &npoints) != 1) {
    //         return 1;
    //     }
    //
    //     /* Allocate memory. */
    //     gkFloat** arr = (gkFloat**)malloc(npoints * sizeof(gkFloat*));
    //     for (int i = 0; i < npoints; i++) {
    //         arr[i] = (gkFloat*)malloc(3 * sizeof(gkFloat));
    //     }
    //
    //     /* Read and store vertices' coordinates. */
    //     for (idx = 0; idx < npoints; idx++) {
    //         if (fscanf_s(fp, "%lf %lf %lf\n", &arr[idx][0], &arr[idx][1], &arr[idx][2]) != 3) {
    //             return 1;
    //         }
    //     }
    //     fclose(fp);
    //
    //     *pts = arr;
    //     *out = idx;
    //
    //     return (0);
    // }

    float get_distance(OpenGJKCase& openGJK_case) {
        gkFloat ** vrtx1 = nullptr, (**vrtx2) = nullptr;

        int npoints_bd1 = 2;
        vrtx1 = (gkFloat**)malloc(npoints_bd1 * sizeof(gkFloat*));
        for (int i = 0; i < npoints_bd1; i++) {
            vrtx1[i] = (gkFloat*)malloc(3 * sizeof(gkFloat));
        }
        vrtx1[0][0] = 0.0;
        vrtx1[0][1] = 0.0;
        vrtx1[0][2] = 0.0;
        vrtx1[1][0] = 0.0;
        vrtx1[1][1] = 0.0;
        vrtx1[1][2] = 2.0;

        int npoints_bd2 = 2;
        vrtx2 = (gkFloat**)malloc(npoints_bd2 * sizeof(gkFloat*));
        for (int i = 0; i < npoints_bd2; i++) {
            vrtx2[i] = (gkFloat*)malloc(3 * sizeof(gkFloat));
        }
        vrtx2[0][0] = 2.0;
        vrtx2[0][1] = 0.0;
        vrtx2[0][2] = 0.0;
        vrtx2[1][0] = 2.0;
        vrtx2[1][1] = 0.0;
        vrtx2[1][2] = 2.0;

        gkPolytope bd1;
        bd1.coord = vrtx1;
        bd1.numpoints = 2;
        bd1.type = Mesh;

        gkPolytope bd2;
        bd2.coord = vrtx2;
        bd2.numpoints = 2;
        bd2.type = Mesh;

        /* Initialize simplex as empty */
        gkSimplex s;
        s.nvrtx = 0;

        /* For importing openGJK this is Step 3: invoke the GJK procedure. */
        /* Compute squared distance using GJK algorithm. */
        gkFloat dd;
        dd = compute_minimum_distance(bd1, bd2, &s);

        return static_cast<float>(dd);
    }
}
