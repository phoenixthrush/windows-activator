$ErrorActionPreference = "Stop"
Set-StrictMode -Version Latest

$release = Invoke-RestMethod -Uri "https://api.github.com/repos/microsoft/winget-cli/releases/latest"

$bundle = $release.assets |
    Where-Object { $_.browser_download_url -match "\.msixbundle$" } |
    Select-Object -First 1

if (-not $bundle) {
    throw "The latest winget release has no MSIX bundle."
}

$installer = Join-Path $env:TEMP "winget-setup.msixbundle"

try {
    Invoke-WebRequest -Uri $bundle.browser_download_url -OutFile $installer
    Add-AppxPackage -Path $installer
} finally {
    Remove-Item $installer -Force -ErrorAction SilentlyContinue
}
