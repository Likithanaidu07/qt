@echo off
REM Set Qt installation path (Change this to your actual Qt path)
set QT_PATH=C:\Qt\6.6.3\msvc2019_64

REM Set project directory (Change this to your project path)
set PROJECT_PATH=C:\Users\Tester\Documents\GitHub\ui-qt

REM Set shared build location
set INSTALL_DIR=\\BuildServer\QtBuilds

REM Open VS Developer Command Prompt
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

REM Set up the Qt environment
call "%QT_PATH%\bin\qtenv2.bat"

REM Navigate to the project directory
cd /d "%PROJECT_PATH%"

REM Run qmake to generate Makefiles
qmake -r project.pro

REM Build the project using nmake
nmake

REM Check if the build was successful
if %ERRORLEVEL% neq 0 (
    echo Build failed!
    exit /b %ERRORLEVEL%
)

echo Build completed successfully!

REM Copy the final build to the shared folder
xcopy /E /I /Y "%PROJECT_PATH%\release\*.exe" "%INSTALL_DIR%"


REM Verify if copy was successful
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Copy to shared folder failed!
    exit /b %ERRORLEVEL%
)

echo Build and deployment completed successfully!
