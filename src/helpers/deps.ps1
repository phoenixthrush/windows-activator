Write-Host "Verifying that all dependencies are installed"

# TODO: gather all dependencies here
$packages = @(
    'ezwinports.make'
)

foreach ($package in $packages) {
    $packageInstalled = (winget list | Select-String -Pattern $package)

    if (-not $packageInstalled) {
        $wingetCommand = "winget install $package"
        Invoke-Expression $wingetCommand
    }
}

# This is probably not needed since a C compiler should already be installed above
# The path should be patched dynamically in the CMakeLists.txt under the BUILD_COMMAND ohook section
<#
$releaseUrl = 'https://api.github.com/repos/brechtsanders/winlibs_mingw/releases/latest'
$downloadLinks = curl.exe -s $releaseUrl | Select-String -Pattern 'browser_download_url' | ForEach-Object { $_.ToString() } | Where-Object { $_ -notmatch 'llvm|7z|sha256|sha512' }

foreach ($link in $downloadLinks) {
    $url = $link -replace '\"browser_download_url\": \"', '' -replace '\"', ''
    $filename = if ($url -like '*i686*') { 'winlibs-i686.zip' } else { 'winlibs-x86_64.zip' }
    $dir = Join-Path $env:APPDATA 'winlibs'

    if (-not (Test-Path $dir)) {
        New-Item -Path $dir -ItemType Directory
    }

    $outFile = Join-Path $dir $filename
    Invoke-WebRequest -Uri $url -OutFile $outFile
}
#>
