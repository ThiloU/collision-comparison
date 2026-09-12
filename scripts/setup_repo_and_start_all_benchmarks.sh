#!/bin/bash
source ~/.bashrc

source /opt/miniconda/bin/activate collision_env

bash scripts/repo_setup.sh

# clean up old results if an earlier benchmark was interrupted:
rm -rf "compare-python/pybullet_result.json"
rm -rf "compare-python/distance3d_result.json"
rm -rf "compare-cpp/cpp_result.json"
rm -rf "compare-rs/target/criterion"

# Run all benchmarks and compile results into CSV files:

bash scripts/benchmarks/benchmark_uc1_ur10.sh
python3 compare-python/analyze_new/compile_CSV_from_results.py results data/uc1_ur10_collision

rm -rf results/*

bash scripts/benchmarks/benchmark_complex_scene.sh
python3 compare-python/analyze_new/compile_CSV_from_results.py results data/complex_env_dual_arm_collision

rm -rf results/*

bash scripts/benchmarks/benchmark_complex_scene_max256verts.sh
python3 compare-python/analyze_new/compile_CSV_from_results.py results data/complex_env_dual_arm_collision_max256verts

rm -rf results/*

bash scripts/benchmarks/benchmark_icospheres.sh
python3 compare-python/analyze_new/compile_CSV_from_results.py results data/icospheres_of_different_vertex_counts

rm -rf results/*

bash scripts/benchmarks/benchmark_high_vertex_count_icospheres.sh
python3 compare-python/analyze_new/compile_CSV_from_results.py results data/icospheres_of_different_high_vertex_counts

rm -rf results/*