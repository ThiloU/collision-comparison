#!/bin/bash
source ~/.bashrc

source /opt/miniconda/bin/activate collision_env

# temporary: better move this to the Dockerfile
apt update -y
apt install -y unzip
pip install open3d

mkdir -p results

if [ ! -d "data/complex_env_dual_arm_collision" ]; then
  echo "Unzipping benchmark data..."
  cd data \
    && unzip -q complex_env_dual_arm_collision.zip \
    && cd ..
fi

# Clone algorithms not already in repo:
git clone https://github.com/MaartenBehn/JoltPhysics.git
git clone https://github.com/danfis/libccd.git
git clone https://github.com/MaartenBehn/bullet3.git
git clone https://github.com/MaartenBehn/hpp-fcl.git

git clone https://github.com/AlexanderFabisch/distance3d.git

# Clone dependencies for the C++ part of the benchmarks:
git clone https://github.com/nlohmann/json.git
git clone https://github.com/martinus/nanobench.git
git clone https://github.com/g-truc/glm.git

# Compile algorithms:
cd JoltPhysics/Build \
 && sh ./cmake_linux_clang_gcc.sh Distribution \
 && cd Linux_Distribution \
 && make -j 8 \
 && cd ../../..
pwd
cd libccd \
 && mkdir -p build && cd build \
 && cmake -G "Unix Makefiles" .. \
 && make \
 && cd ../..

cd hpp-fcl \
 && git submodule update --init \
 && cd ..

cd openGJK \
 && cmake -E make_directory build \
 && cmake -E chdir build cmake -DCMAKE_BUILD_TYPE=Release -G Ninja .. \
 && cmake --build build \
 && cd ..

pip install -e ./distance3d

mkdir -p compare-cpp/build_release/ \
 && bash scripts/compile/compile_compare_release.sh

rm -rf compare-rs/target
