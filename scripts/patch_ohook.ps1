$ErrorActionPreference = "Stop"
Set-StrictMode -Version Latest

# ohook's Makefile contains a fixed MinGW path. Replace it with the compiler
# selected by the current environment, then let CMake handle final stripping.
$gcc = Get-Command "gcc"
$mingwPath = (Split-Path $gcc.Source) + "\"
$mingwPath = $mingwPath -replace "\\", "/"

$makefile = Get-Content "Makefile"
$makefile = $makefile -replace "C:/mingw64/bin/", $mingwPath
$makefile = $makefile | Where-Object { $_ -notmatch "^\s*strip -s" }
$makefile | Set-Content "Makefile"
