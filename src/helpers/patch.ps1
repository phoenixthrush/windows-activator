# $mingw32Path = Split-Path (Get-Command gcc).Source
# $mingw32Path = $mingw32Path + '\'
# $mingw32Path = $mingw32Path -replace '\\', '/'

$mingw64Path = Split-Path (Get-Command gcc).Source
$mingw64Path = $mingw64Path + '\'
$mingw64Path = $mingw64Path -replace '\\', '/'

# (Get-Content Makefile) -replace "C:/mingw32/bin/", $mingw32Path -replace "C:/mingw64/bin/", $mingw64Path | Set-Content Makefile
(Get-Content Makefile) -replace "C:/mingw64/bin/", $mingw64Path | Set-Content Makefile
(Get-Content Makefile) | Where-Object {$_ -notmatch '^\s*strip -s'} | Set-Content Makefile