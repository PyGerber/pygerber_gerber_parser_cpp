#!/bin/bash
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
