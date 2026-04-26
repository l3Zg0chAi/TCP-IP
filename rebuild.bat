@echo off
cd /d "%~dp0"

echo ===== CLEAN =====
rmdir /s /q "build" 2>nul

echo ===== CONFIGURE =====
cmake -S . -B build

if errorlevel 1 (
    echo CMake configure loi.
    pause
    exit /b 1
)

echo ===== BUILD =====
cmake --build build

if errorlevel 1 (
    echo Build loi.
    pause
    exit /b 1
)

echo ===== DONE =====
pause