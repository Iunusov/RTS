git submodule init
git submodule update

mkdir build
cd build

cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel 8 --config Release

cd ..

