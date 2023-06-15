#!/usr/bin/env bash
set -e
build_dir=$1
source ../misc/valgrind_helpers

cd $build_dir/t-infinity/src/unit_tests
mpirun -np 1 valgrind ${DEFAULT_ARGS} --gen-suppressions=all ./infinity_unit_tests -d yes
