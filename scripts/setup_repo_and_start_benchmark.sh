#!/bin/bash
source ~/.bashrc

source /opt/miniconda/bin/activate collision_env

bash scripts/repo_setup.sh

# clean up old results if an earlier benchmark was interrupted:
rm -rf "compare-python/pybullet_result.json"
rm -rf "compare-python/distance3d_result.json"
rm -rf "compare-cpp/cpp_result.json"
rm -rf "compare-rs/target/criterion"

bash scripts/benchmarks/benchmark_complex_scene.sh