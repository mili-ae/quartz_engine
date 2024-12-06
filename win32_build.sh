#!/bin/bash

timestamp=$(date +%s)

# Windows
# Engine
libs="-lopengl32 -lengine/lib/glew32 -lengine/lib/glfw3dll -lengine/lib/assimp-vc143-mt"
warnings="-Wno-writable-strings -Wno-format-security -Wno-deprecated-declarations -Wno-switch -Wno-address-of-temporary"
includes="-Iengine/third_party -Iengine/third_party/Include -Iengine/third_party/glew -Iengine/third_party/glfw -Iengine/third_party/glm -Iengine/third_party/assimp"
build_dir="build/win64"

rm -rf $build_dir
mkdir -p $build_dir

clang++ $includes -g engine/src/engine.cpp -shared -o $build_dir/quartz_engine.dll $libs $warnings -MJ compile_commands.json
cp engine/lib/glew32.dll build/win64
cp engine/lib/glfw3.dll build/win64
cp engine/lib/assimp-vc143-mt.dll build/win64

# Sandbox
s_includes="-Iengine/src -Iengine/third_party/assimp"
clang++ $s_includes -g sandbox/src/main.cpp -o $build_dir/sandbox.exe -l$build_dir/quartz_engine $libs $warnings
cp -r sandbox/assets build/win64