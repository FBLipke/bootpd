@echo off
REM bootpd Build Script for Windows (VS Build Tools)
REM Usage: build.cmd [Debug|Release|clean]

set CONFIG=%1
if "%CONFIG%"=="" set CONFIG=Debug

REM Handle clean
if "%CONFIG%"=="clean" (
    echo === Cleaning build directory ===
    if exist build rmdir /s /q build
    echo [OK] Build directory cleaned
    exit /b 0
)

echo === bootpd Build Script ===
echo Configuration: %CONFIG%
echo.

REM Initialize VS Build Tools
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64

REM Verify compiler
where cl.exe >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo [ERROR] cl.exe not found!
    exit /b 1
)

echo [OK] Compiler found

set BUILD_DIR=%~dp0build
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

cd /d "%BUILD_DIR%"

echo.
echo === Running CMake ===
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=%CONFIG% ..

if %ERRORLEVEL% neq 0 (
    echo [ERROR] CMake failed!
    exit /b 1
)

echo.
echo === Building ===
cmake --build . --config %CONFIG%

if %ERRORLEVEL% neq 0 (
    echo [ERROR] Build failed!
    exit /b 1
)

echo.
echo === Build Complete ===
