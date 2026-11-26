@echo off
REM ============================================================================
REM SPDX-License-Identifier: Apache-2.0
REM Copyright 2026 Junya005
REM
REM File:         Setup.bat
REM Description:  Sets up dependencies and generates build files for YamanekoEngine.
REM Prerequisites: Git, CMake (3.20+), Visual Studio 2022
REM Usage:        Setup.bat
REM ============================================================================

setlocal DisableDelayedExpansion
cd /d "%~dp0"

echo ========================================
echo  YamanekoEngine Setup
echo ========================================

@REM Check needed commands (git / cmake)
where git >nul 2>&1
if errorlevel 1 (
    echo [ERROR] Git is not found. Install Git and make sure it is in your PATH.
    pause
    exit /b 1
)

where cmake >nul 2>&1
if errorlevel 1 (
    echo [ERROR] CMake is not found. Check your CMake installation or Visual Studio command environment.
    pause
    exit /b 1
)

@REM Target vcpkg commit hash (should match the submodule / builtin-baseline)
set "VCPKG_COMMIT=93ad765c7b66ab9ce97469ec23a90b15c3a14ee5"

@REM Initialize submodules or clone vcpkg if .git is not present
if exist ".git" (
    echo [1/3] Initializing submodules...
    git submodule update --init
    if errorlevel 1 (
        echo [ERROR] Failed to initialize submodules.
        pause
        exit /b 1
    )
) else (
    echo [1/3] .git not found ^(ZIP download^). Cloning vcpkg...
    if not exist "vcpkg\bootstrap-vcpkg.bat" (
        if exist "vcpkg" rmdir /s /q "vcpkg"
        git clone https://github.com/microsoft/vcpkg.git vcpkg
        if errorlevel 1 (
            echo [ERROR] Failed to clone vcpkg.
            pause
            exit /b 1
        )
    )
    git -C vcpkg checkout %VCPKG_COMMIT%
    if errorlevel 1 (
        echo [ERROR] Failed to checkout vcpkg commit %VCPKG_COMMIT%.
        pause
        exit /b 1
    )
)

@REM Bootstrap vcpkg
if not exist "vcpkg\vcpkg.exe" (
    echo [2/3] Bootstrapping vcpkg...
    call "vcpkg\bootstrap-vcpkg.bat" -disableMetrics
    if errorlevel 1 (
        echo [ERROR] Failed to bootstrap vcpkg.
        pause
        exit /b 1
    )
) else (
    echo [2/3] vcpkg.exe already exists. Skipping.
)

@REM Create binary cache directory and run CMake Configure
set "CACHE_DIR=%LOCALAPPDATA%\YamanekoEngine\vcpkg_cache"
if not exist "%CACHE_DIR%" mkdir "%CACHE_DIR%"

echo [3/3] Running CMake Configure and installing dependencies...
cmake --preset vcpkg-windows
if errorlevel 1 (
    echo [ERROR] CMake Configure failed.
    pause
    exit /b 1
)

echo.
echo ========================================
echo  Setup completed successfully!
echo  You can run Build.bat to build the project.
echo ========================================
pause
