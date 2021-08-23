git submodule init
git submodule update

cmake -B ./build -DCMAKE_BUILD_TYPE=Release
cmake --build ./build --parallel 8 --config Release
