set PATH=%PATH%;C:\Qt\Tools\CMake_64\bin;C:\Qt\Tools\Ninja
C:
cd C:\Qt\6.7.2\Src\qtbase\src\plugins\sqldrivers
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
call C:\Qt\6.7.2\msvc2019_64\bin\qt-cmake.bat -G "Ninja Multi-Config" . -DMySQL_INCLUDE_DIR="C:\Program Files\MySQL\MySQL Server 8.4\include" -DMySQL_LIBRARY="C:\Program Files\MySQL\MySQL Server 8.4\lib\libmysql.lib" -DCMAKE_INSTALL_PREFIX="C:\Qt\6.7.2\msvc2019_64" -DCMAKE_CONFIGURATION_TYPES=Release;Debug
ninja
ninja install
pause