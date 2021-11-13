#!/bin/sh
export LC_ALL=C
set -e
set -x
version=0.13.0
file_name="prometheus-cpp-with-submodules.tar.gz"
download_path=https://github.com/jupp0r/prometheus-cpp/releases/download/v${version}/${file_name}
sha256_hash=c8e4b479fc3af24dc0a0b4592010f88f72b31620a72bf50a6ef2e8734dba8358

CXX=${CXX-"clang++"}
CC=${CC-"clang"}
CXXFLAGS=${CXXFLAGS-"-std=c++17 -O -fPIC"}
CFLAGS="-g -fPIC"
PROMETHEUS_DIR=ci/scratch/prometheus
mkdir -p $PROMETHEUS_DIR
[ -f "$PROMETHEUS_DIR/$file_name" ] || curl -L --fail $download_path > $PROMETHEUS_DIR/$file_name
cd $PROMETHEUS_DIR
echo "$sha256_hash $file_name" | sha256sum -c
rm -rf prometheus-cpp-with-submodules
tar zxf $file_name
cd prometheus-cpp-with-submodules
cmake  . -DOVERRIDE_CXX_STANDARD_FLAGS=OFF -DCMAKE_CXX_STANDARD=17 \
   -DCMAKE_CXX_COMPILER="${CXX}" \
   -DCMAKE_C_COMPILER="${CC}" \
   -DCMAKE_CXX_FLAGS="${CXXFLAGS}" \
   -DCMAKE_C_FLAGS="${CFLAGS}" \
   -DENABLE_COMPRESSION=OFF -DENABLE_PUSH=OFF -DBUILD_SHARED_LIBS=OFF -DENABLE_TESTING=OFF -DCMAKE_INSTALL_PREFIX=/usr

cmake --build . --verbose
make install
