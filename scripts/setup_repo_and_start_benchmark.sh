#!/bin/bash
source ~/.bashrc

source /opt/miniconda/bin/activate collision_env

bash scripts/repo_setup.sh

bash scripts/benchmarks/benchmark_complex_scene.sh