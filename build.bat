@echo off
setlocal EnableDelayedExpansion

REM =======================
REM Configuration
REM =======================
set QT_PATH=C:\Qt\6.6.3\msvc2019_64
set PROJECT_PATH=C:\Users\Tester\Documents\GitHub\ui-qt
set INSTALL_DIR=\\BuildServer\QtBuilds
set VS_DEV_CMD="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
set PRO_FILE=new_speedtrade.pro

REM =======================
REM Setup Environment
REM =======================
echo Setting up Visual Studio environment...
call %VS_DEV_CMD%
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Failed to set up Visual Studio environment!
    exit /b %ERRORLEVEL%
)

echo Setting up Qt environment...
call "%QT_PATH%\bin\qtenv2.bat"
set PATH=%QT_PATH%\bin;%PATH%

REM =======================
REM Navigate to Project Directory
REM =======================
cd /d "%PROJECT_PATH%"
if not exist "%PRO_FILE%" (
    echo [ERROR] Project file "%PRO_FILE%" not found!
    exit /b 1
)

REM =======================
REM Increment Version in .pro File
REM =======================
echo Checking current version...
for /f "tokens=2 delims==" %%V in ('findstr /C:"VERSION =" "%PRO_FILE%"') do set CUR_VERSION=%%V
if "%CUR_VERSION%"=="" (
    echo [ERROR] VERSION not found in .pro file!
    exit /b 1
)

for /f "tokens=1,2,3 delims=." %%a in ("%CUR_VERSION%") do (
    set MAJOR=%%a
    set MINOR=%%b
    set PATCH=%%c
)

set /a PATCH+=1
set NEW_VERSION=%MAJOR%.%MINOR%.%PATCH%

echo Updating version to %NEW_VERSION%...
(
    for /f "usebackq delims=" %%L in ("%PRO_FILE%") do (
        echo %%L | findstr /r "^VERSION =" >nul && echo VERSION = %NEW_VERSION% || echo %%L
    )
) > "%PRO_FILE%.tmp"
move /Y "%PRO_FILE%.tmp" "%PRO_FILE%"

REM =======================
REM Clean and Build
REM =======================
echo Cleaning old builds...
rmdir /s /q release 2>nul
rmdir /s /q debug 2>nul

echo Running QMake...
qmake -r "%PRO_FILE%"
if %ERRORLEVEL% neq 0 (
    echo [ERROR] QMake failed!
    exit /b %ERRORLEVEL%
)

echo Running NMake...
nmake
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Build failed!
    exit /b %ERRORLEVEL%
)

echo Build completed successfully!

REM =======================
REM Copy Build to Shared Folder
REM =======================
echo Copying build files to %INSTALL_DIR%...
xcopy /E /I /Y "release\*.exe" "%INSTALL_DIR%"

if %ERRORLEVEL% neq 0 (
    echo [ERROR] Copy to shared folder failed!
    exit /b %ERRORLEVEL%
)

echo Build and deployment completed successfully! 🚀
exit /b 0
