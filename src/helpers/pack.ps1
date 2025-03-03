$activatorPath = "build/bin/activator.exe"

if (Test-Path $activatorPath) {
    Write-Host "$activatorPath found, compressing with UPX..."
    if (Get-Command upx -ErrorAction SilentlyContinue) {
        $processorArchitecture = [System.Environment]::GetEnvironmentVariable("PROCESSOR_ARCHITECTURE")
        $processorIdentifier = [System.Environment]::GetEnvironmentVariable("PROCESSOR_IDENTIFIER")

        if ($processorArchitecture -eq "ARM64" -or $processorIdentifier -match "ARM" -or $processorIdentifier -match "AArch64") {
            Write-Host "Skipping UPX compression as the system is ARM-based."
        } else {
            Write-Host "Running UPX compression..."
            upx --best $activatorPath
        }
    } else {
        Write-Host "UPX is not installed."
    }
} else {
    Write-Host "$activatorPath not found."
}
