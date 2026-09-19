import os
import json
from pathlib import Path
import matplotlib.pyplot as plt
import miniball
import numpy as np
from src import load_test_file
from src.test_file import load_mesh_from_obj

# BENCHMARK_DIR = Path("../data/icospheres_of_different_high_vertex_counts")
BENCHMARK_DIR = Path("../data/complex_env_dual_arm_collision")

# approximate distance interval in which Nesterov Accelerated HPP-FCL beats the default variant:
DIST_INTERVAL = [-0.002, 0.05]

print("Showing metadata for benchmark", BENCHMARK_DIR)

distances = []
num_cases_in_distance_interval = 0

directory_contents = os.scandir(BENCHMARK_DIR)
for entry in directory_contents:
    if not entry.is_file(): continue

    with open(entry.path) as json_file:
        cases = json.load(json_file)

    for case in cases:
        distances.append(case["signed_distance"])
        # distances.append(case["collider1"]["mesh_path"])
        if DIST_INTERVAL[0] <= case["signed_distance"] <= DIST_INTERVAL[1]:
            num_cases_in_distance_interval += 1

print(f"Number of cases in distance interval {DIST_INTERVAL}: {num_cases_in_distance_interval}  = {(num_cases_in_distance_interval/len(distances))*100:.2f}%")
# plt.scatter(distances, y=np.arange(len(distances)) // 40)


counts, bins = np.histogram(distances, bins=30)
plt.stairs(counts, bins)
plt.title("Histogram of distance distribution")
plt.xlabel("Distance")
plt.ylabel("Count")
plt.show()


meshes_path = Path(BENCHMARK_DIR, "meshes")

if meshes_path.exists():
    print("Calculating radii of enclosing spheres, number of vertices and number of faces for all meshes...")
    radii = []
    n_vertices = []
    n_faces = []
    mesh_names = []
    for mesh in os.listdir(meshes_path):
        verts, tris = load_mesh_from_obj(Path(meshes_path, mesh))

        vertices = np.asarray(verts, dtype=np.float64)
        c, r2 = miniball.get_bounding_ball(vertices, rng=np.random.default_rng(42))
        radius = np.sqrt(r2)
        radii.append(radius)
        mesh_names.append(mesh)
        n_vertices.append(len(verts))
        n_faces.append(len(tris))


    print(f"Average Radius: {np.mean(radii):.4f}m")
    print(f"Minimum Radius: {np.min(radii):.4f}m in mesh: {mesh_names[np.argmin(radii)]}")
    print(f"Maximum Radius: {np.max(radii):.4f}m in mesh: {mesh_names[np.argmax(radii)]}")

    print(f"Average # of vertices: {np.mean(n_vertices):.0f}")
    print(f"Minimum # of vertices: {np.min(n_vertices):.0f} in mesh: {mesh_names[np.argmin(n_vertices)]}")
    print(f"Maximum # of vertices: {np.max(n_vertices):.0f} in mesh: {mesh_names[np.argmax(n_vertices)]}")

    print(f"Average # of faces: {np.mean(n_faces):.0f}")
    print(f"Minimum # of faces: {np.min(n_faces):.0f} in mesh: {mesh_names[np.argmin(n_faces)]}")
    print(f"Maximum # of faces: {np.max(n_faces):.0f} in mesh: {mesh_names[np.argmax(n_faces)]}")
