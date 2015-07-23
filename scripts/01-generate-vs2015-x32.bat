cd ..
if not exist "build" mkdir build
cd build
cmake -G "Visual Studio 14 2015" ..
