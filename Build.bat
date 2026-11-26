@echo off
REM ============================================================================
REM SPDX-License-Identifier: Apache-2.0
REM Copyright 2026 Junya005
REM
REM File:         Build.bat
REM Description:  Builds YamanekoEngine without opening the Visual Studio IDE.
REM Prerequisites: Run Setup.bat first.
REM Usage:        Build.bat [Debug|Release]
REM ============================================================================

setlocal
cd /d "%~dp0"

set "CONFIG=%~1"
if "%CONFIG%"=="" set "CONFIG=Debug"

@REM Check ran Setup.bat and build directory exists
if not exist "build\CMakeCache.txt" (
    echo [ERROR] Build directory is not configured. Please run Setup.bat first.
    pause
    exit /b 1
)

echo ========================================
echo  YamanekoEngine Build (%CONFIG%)
echo ========================================

@REM Build the project based on the specified configuration
if /i "%CONFIG%"=="Release" (
    cmake --build --preset vcpkg-windows-release
) else if /i "%CONFIG%"=="Debug" (
    cmake --build --preset vcpkg-windows-debug
) else (
    echo [ERROR] Unknown configuration: %CONFIG%. Specify Debug or Release.
    pause
    exit /b 1
)

@REM Check if the build command succeeded
if errorlevel 1 (
    echo [ERROR] Build failed.
    pause
    exit /b 1
)

echo.
echo [SUCCESS] %CONFIG% build completed successfully!
pause
