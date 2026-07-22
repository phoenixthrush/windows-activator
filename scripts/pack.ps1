$ErrorActionPreference = "Stop"
Set-StrictMode -Version Latest

$activatorPath = "build/bin/activator.exe"
$webviewLibrary = "build/bin/libwebview.dll"

if (-not (Test-Path $activatorPath)) {
    Write-Host "No activator binary was found."
    exit 0
}

$architecture = [System.Environment]::GetEnvironmentVariable(
    "PROCESSOR_ARCHITECTURE"
)
$processor = [System.Environment]::GetEnvironmentVariable(
    "PROCESSOR_IDENTIFIER"
)
$isArm = $architecture -eq "ARM64" -or $processor -match "ARM|AArch64"
$upx = Get-Command "upx" -ErrorAction SilentlyContinue

if (-not $upx) {
    Write-Host "UPX is not installed; leaving the binary uncompressed."
} elseif ($isArm) {
    Write-Host "Skipping UPX on ARM."
} else {
    Write-Host "Compressing $activatorPath..."
    upx --best $activatorPath
}

if (Test-Path $webviewLibrary) {
    Remove-Item $webviewLibrary -Force
}
