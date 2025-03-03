@echo off

where yourcommand.exe >nul 2>nul

if %errorlevel% neq 0 (
    echo Winget not installed. Installing...
    powershell -ExecutionPolicy Bypass -File winget.ps1
) else (
    echo Winget found.
)

winget install ezwinports.make