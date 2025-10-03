#/usr/bin/env bash
cmake -B build_vector -DMODE=DEBUG_VECTOR
cmake --build build_vector
echo
echo
echo "################################################################################"
echo "Errors will start appearing from here on if found :"
echo "##########"
./build_vector/vec_debug.out
