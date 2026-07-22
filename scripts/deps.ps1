$ErrorActionPreference = "Stop"
Set-StrictMode -Version Latest

Write-Host "Checking build dependencies..."

$packages = [ordered]@{
    "make"   = "ezwinports.make"
    "python" = "Python.Python.3.13"
    "ffmpeg" = "Gyan.FFmpeg.Shared"
    "cmake"  = "Kitware.CMake"
    "ninja"  = "Ninja-build.Ninja"
    "gcc"    = "MartinStorsjo.LLVM-MinGW.UCRT"
    "upx"    = "UPX.UPX"
    "git"    = "Git.Git"
}

$installedPackage = $false

foreach ($commandName in $packages.Keys) {
    $command = Get-Command $commandName -ErrorAction SilentlyContinue

    # Windows' Store alias looks like Python but cannot run build scripts.
    if ($commandName -eq "python" -and $command -and $command.Path -match "WindowsApps") {
        $command = $null
    }

    if (-not $command) {
        $packageId = $packages[$commandName]
        Write-Host "Installing $packageId..."

        $wingetArguments = @(
            "install",
            "--id", $packageId,
            "--exact",
            "--accept-package-agreements",
            "--accept-source-agreements"
        )
        & winget @wingetArguments
        $installedPackage = $true
    }
}

if ($installedPackage) {
    Write-Host "Dependencies were installed. Restart this shell, then run make again."
    exit 1
}

if (-not (Get-Command "yt-dlp" -ErrorAction SilentlyContinue)) {
    python -m pip install yt-dlp
}

$audioPath = "site/assets/audio/keygen-Uh-p3TOIrOc.mp3"

if (Test-Path $audioPath) {
    Write-Host "Audio asset already exists."
    exit 0
}

Write-Host "Downloading the fallback audio asset..."
New-Item -ItemType Directory -Force -Path (Split-Path $audioPath) | Out-Null

$audioArguments = @(
    "--format", "bestaudio",
    "--extract-audio",
    "--audio-format", "mp3",
    "--output", $audioPath,
    "https://www.youtube.com/watch?v=tPY-I3RX10c"
)
& yt-dlp @audioArguments
