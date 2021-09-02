git submodule update --init --force --remote  --recursive

mkdir build
cd build

cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel 8 --config Release

cd ..

