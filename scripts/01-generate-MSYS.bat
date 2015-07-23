cd ..
if not exist "build" mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -G "MSYS Makefiles" ..
