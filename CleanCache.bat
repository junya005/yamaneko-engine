@echo off
REM ============================================================================
REM SPDX-License-Identifier: Apache-2.0
REM Copyright 2026 Junya005
REM
REM File:         CleanCache.bat
REM Description:  Cleans local build artifacts and optionally vcpkg binary cache.
REM Usage:        CleanCache.bat
REM ============================================================================

setlocal DisableDelayedExpansion
cd /d "%~dp0" || exit /b 1
setlocal EnableDelayedExpansion

echo ========================================
echo  YamanekoEngine Cache Cleaner
echo ========================================

@REM Prompt user for cache cleaning options
echo 1. Delete local build artifacts (build, out, vcpkg_installed)
echo 2. Delete all above + vcpkg binary cache (.zip)
echo 3. Cancel
echo.
choice /c 123 /m "Select an option"

@REM Handle user choice
if errorlevel 3 exit /b 0
set "DELETE_BIN_CACHE=0"
if errorlevel 2 set "DELETE_BIN_CACHE=1"

@REM Confirm deletion of local build artifacts
echo.
echo Deleting local build artifacts...
if exist "build" rmdir /s /q "build"
if exist "out" rmdir /s /q "out"
if exist "vcpkg_installed" rmdir /s /q "vcpkg_installed"

@REM Confirm deletion of binary cache if applicable
if "%DELETE_BIN_CACHE%"=="1" (
    set "CACHE_DIR=!LOCALAPPDATA!\YamanekoEngine\vcpkg_cache"
    echo Deleting binary cache: !CACHE_DIR!
    if exist "!CACHE_DIR!" rmdir /s /q "!CACHE_DIR!"
)

echo.
echo Cleanup completed. Run Setup.bat to reconfigure the project.
pause
