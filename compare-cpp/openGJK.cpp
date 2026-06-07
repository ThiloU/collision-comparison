#include "openGJK.h"

#include <iostream>


#include "openGJK_impl.h"


#include <math.h>

namespace compare::OpenGJK {
    void get_transform(const Collider& collider, gkBody& gkBody){
        for (int x = 0; x < 3; x++){
            for (int y = 0; y < 3; y++){
                gkBody.rotation_mat[x][y] = collider.colliderToOrigen[x][y];
            }
        }
        gkBody.translation[0] = collider.colliderToOrigen[0][3];
        gkBody.translation[1] = collider.colliderToOrigen[1][3];
        gkBody.translation[2] = collider.colliderToOrigen[2][3];
    }

    void get_collider(const Collider& collider, gkBody& gkBody){
        if (collider.type == ColliderType::Sphere){
            gkBody.type = Sphere;
            gkBody.radius = get_radius(collider);
        }

        if (collider.type == ColliderType::Cylinder){
            gkBody.type = Cylinder;
            gkBody.radius = get_radius(collider);
            gkBody.height = get_height(collider);
        }

        if (collider.type == ColliderType::Capsule){
            gkBody.type = Capsule;
            gkBody.radius = get_radius(collider);
            gkBody.height = get_height(collider);
        }

        if (collider.type == ColliderType::Box){
            gkBody.type = Box;
            gkBody.half_x_length = get_size_x(collider) / 2.0;
            gkBody.half_y_length = get_size_y(collider) / 2.0;
            gkBody.half_z_length = get_size_z(collider) / 2.0;
        }

        if (collider.type == ColliderType::Mesh){
            gkBody.type = Mesh;
            gkBody.numpoints = get_vertex_count(collider);

            gkBody.coord = static_cast<gkFloat**>(malloc(gkBody.numpoints * sizeof(gkFloat*)));
            for (int i = 0; i < gkBody.numpoints; i++) {
                gkBody.coord[i] = static_cast<gkFloat*>(malloc(3 * sizeof(gkFloat)));
            }
            for (int i = 0; i < gkBody.numpoints; i++){
                gkBody.coord[i][0] = collider.vertecies[i].x;
                gkBody.coord[i][1] = collider.vertecies[i].y;
                gkBody.coord[i][2] = collider.vertecies[i].z;
            }
        }
    }

    void get_case(const Collider& collider0, const Collider& collider1, OpenGJKCase& openGJK_case){
        get_collider(collider0, openGJK_case.collider0);
        get_collider(collider1, openGJK_case.collider1);

        get_transform(collider0, openGJK_case.collider0);
        get_transform(collider1, openGJK_case.collider1);
    }

    void get_cases(const Case* base_cases, OpenGJKCase* openGJK_cases, int length){

        for (int i = 0; i < length; i++) {

            const auto base_case = base_cases[i];
            get_case(base_case.collider0, base_case.collider1, openGJK_cases[i]);

        }
    }

    float get_distance(const OpenGJKCase& openGJK_case) {
        /* Initialize simplex as empty */
        gkSimplex s;
        s.nvrtx = 0;

        /* Compute distance */
        const gkFloat dd = compute_minimum_distance(openGJK_case.collider0, openGJK_case.collider1, &s);

        return static_cast<float>(dd);
    }
}
