$URL = "https://api.github.com/repos/microsoft/winget-cli/releases/latest"

$URL = (Invoke-WebRequest -Uri $URL).Content | ConvertFrom-Json |
        Select-Object -ExpandProperty "assets" |
        Where-Object "browser_download_url" -Match '.msixbundle' |
        Select-Object -ExpandProperty "browser_download_url"

curl.exe -L $URL -o Setup.msix

Add-AppxPackage -Path "Setup.msix"

Remove-Item "Setup.msix"