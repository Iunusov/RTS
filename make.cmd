git submodule update --init --force --remote  --recursive

mkdir build_dir
cd build_dir

cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .  --config Debug

cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .  --config Release

cd ..

