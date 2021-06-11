
setlocal
SET PATH=%PATH%;C:\Users\t.dallard\Downloads\cmake-3.19.4-win64-x64\bin\
REM cmake --build Build
REM Build/SalieriTest.exe

mkdir Build
cd Build
cmake ../.
cmake --build .
cd ..
REM ./Build/run
.\Build\Debug\SalieriTest.exe