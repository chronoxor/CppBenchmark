C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\Tools\vsvars32.bat 

cd ..
if not exist "build" mkdir build
cd build
cmake -G "Visual Studio 14 2015" ..
