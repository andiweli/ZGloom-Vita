echo "> Building compile-toolchain"
cmake -S . -B build -DVITA_PERF_TWEAKS=ON -DCMAKE_BUILD_TYPE=Release
echo "> Compiling..."
cmake --build build -j
