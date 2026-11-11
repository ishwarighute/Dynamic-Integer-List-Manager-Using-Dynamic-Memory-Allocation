@echo off
echo ==============================================
echo Building Dynamic Integer List Manager Project
echo ==============================================

gcc -Wall -Wextra -std=c11 dynamic_list.c main.c -o list_manager_c.exe
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to compile C list manager.
    exit /b 1
)

gcc -Wall -Wextra -std=c11 dynamic_list.c test_runner.c -o test_runner.exe
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to compile test runner.
    exit /b 1
)

g++ -Wall -Wextra -std=c++17 list_manager.cpp -o list_manager_cpp.exe
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to compile C++ list manager.
    exit /b 1
)

echo.
echo [SUCCESS] Build completed successfully!
echo Executables generated:
echo   - list_manager_c.exe   (Interactive C Application)
echo   - list_manager_cpp.exe (Interactive C++ Application)
echo   - test_runner.exe      (Automated Unit Tests)
echo.
echo Running automated tests...
echo ==============================================
.\test_runner.exe

