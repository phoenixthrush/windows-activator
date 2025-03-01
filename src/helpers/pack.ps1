$activatorPath = "build/bin/activator.exe"

if (Test-Path $activatorPath) {
    Write-Host "$activatorPath found, compressing with UPX..."
    if (Get-Command upx -ErrorAction SilentlyContinue) {
        upx --best $activatorPath
    } else {
        Write-Host "UPX is not installed."
    }
} else {
    Write-Host "$activatorPath not found."
}