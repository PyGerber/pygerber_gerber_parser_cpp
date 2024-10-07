rmdir build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -S . -B build -G Ninja
cmake --build build
