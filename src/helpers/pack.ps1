$activatorPath = "build/bin/activator.exe"

if (Test-Path $activatorPath) {
    Write-Host "$activatorPath found, compressing with UPX..."
    if (Get-Command upx -ErrorAction SilentlyContinue) {
        $processorArchitecture = [System.Environment]::GetEnvironmentVariable("PROCESSOR_ARCHITECTURE")

        if ($processorArchitecture -eq "ARM64") {
            Write-Host "Skipping UPX compression as the system is ARM-based."
        } else {
            upx --best $activatorPath
        }
    } else {
        Write-Host "UPX is not installed."
    }

    $commit_hash = git rev-parse --short HEAD
    $newFileName = "build/bin/activator.$commit_hash.exe"
    Rename-Item $activatorPath $newFileName
    Write-Host "File renamed to $newFileName"
} else {
    Write-Host "$activatorPath not found."
}