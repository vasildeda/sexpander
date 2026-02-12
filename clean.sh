export CMAKE_BUILD_PARALLEL_LEVEL=$(sysctl -n hw.ncpu)
rm -rf build && cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
ln -sf build/compile_commands.json .
