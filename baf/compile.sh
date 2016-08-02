#! /bin/sh
# Script prepares installation file structure and executes build commands

BUILD_DIR="bin"

mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}
cmake ${CMAKE_ARGS} -DCMAKE_INSTALL_PREFIX=../${INSTALL_BUILD_DIR} ..
make
