import os
import random

import numpy as np
from scipy.spatial.transform import Rotation

from distance3d.colliders import MeshGraph
from distance3d.hydroelastic_contact._tetra_mesh_creation import make_triangular_icosphere
from src import write_test_file


def rand_unit_vector():
    """
    Generate a 3D unit vector pointing in a random direction
    :return: The vector as numpy array
    """
    theta = rng.uniform(0, 2*np.pi)
    y = rng.uniform(-1,1)
    k = np.sqrt(1-y*y)
    return np.array([k*np.cos(theta), k*np.sin(theta), y])


subdirectory_name = "../data/icospheres_of_different_vertex_counts"
os.makedirs(subdirectory_name + "/meshes", exist_ok=True)

num_vertex_counts = 7
num_random_poses_per_vertex_count = 140
i = 0

# to get reproducible benchmark cases, set the rng seed:
random.seed(42)
rng = np.random.default_rng(42)

# alternatively to subdividing the initial 20 face icosphere,
# this would also give a nice range of 10 vertex counts from 10 to 39810:
# vertex_counts = 10 ** np.arange(1,5, step=0.4)

for subdivisions in range(num_vertex_counts):
    sphere_vertices, sphere_triangles = make_triangular_icosphere(center=np.array([0,0,0]), radius = 0.5, order=subdivisions)

    for _ in range(num_random_poses_per_vertex_count):
        # the first collider is placed at the origin with random rotation:
        transform0 = np.eye(4)
        transform0[:3, :3] = Rotation.random(rng=rng).as_matrix()
        transform0[:3, 3] = np.array([0, 0, 0])

        # the second collider is also rotated randomly:
        transform1 = np.eye(4)
        transform1[:3, :3] = Rotation.random(rng=rng).as_matrix()

        # the second collider also gets translated some random amount between 0.1 and 1.9 units.
        # With the spheres having a diameter of 1 unit, the number of benchmark cases
        # where the spheres intersect will be about equal to the number where they don't.
        transform1[:3, 3] = rand_unit_vector() * rng.uniform(0.1,1.9)

        collider0 = MeshGraph(transform0, sphere_vertices, sphere_triangles)
        collider1 = MeshGraph(transform1, sphere_vertices, sphere_triangles)

        cases = [((f"icosphere_{subdivisions}order", collider0), (f"icosphere_{subdivisions}order", collider1))]

        print(i)
        write_test_file(cases, subdirectory_name, f"icospheres_of_different_vertex_counts_{i}.json", clean_collider_names=False)
        i += 1
