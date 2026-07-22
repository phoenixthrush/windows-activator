@echo off
setlocal

where winget >nul 2>nul
if errorlevel 1 (
    echo Winget not found. Installing...
    powershell -ExecutionPolicy Bypass -File "%~dp0install_winget.ps1"
)

winget install --id ezwinports.make --exact --accept-package-agreements --accept-source-agreements
