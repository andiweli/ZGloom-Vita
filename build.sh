echo "> Building compile-toolchain"
echo "------------------------------------------------------------------------------"
cmake -S . -B build -DVITA_PERF_TWEAKS=ON -DCMAKE_BUILD_TYPE=Release
echo "------------------------------------------------------------------------------"
echo "> Compiling ..."
echo "------------------------------------------------------------------------------"
cmake --build build -j
