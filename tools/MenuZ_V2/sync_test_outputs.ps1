param(
    [ValidateSet('All', 'MenuZ', 'UiAdminZ')]
    [string]$Only = 'All'
)

$ErrorActionPreference = 'Stop'

$OutputRoot = 'E:\DeutschZ\DeutschZServer'
$ResolvedOutputRoot = (Resolve-Path -LiteralPath $OutputRoot).Path
if ($ResolvedOutputRoot -ne $OutputRoot) {
    throw "Unexpected output root: $ResolvedOutputRoot"
}

$Stamp = Get-Date -Format 'yyyyMMdd_HHmmss'
$StagingRoot = Join-Path $env:TEMP "DeutschZ_MenuZ_V2_Output_$Stamp"
$OldRoot = Join-Path $env:TEMP "DeutschZ_MenuZ_V2_Old_$Stamp"
New-Item -ItemType Directory -Force -Path $StagingRoot, $OldRoot | Out-Null

function Copy-Product {
    param(
        [string]$Product,
        [string]$BuildRoot,
        [string]$SourceRoot,
        [string]$SettingsSource,
        [string]$SettingsName,
        [string]$TestGuideSource
    )

    $Stage = Join-Path $StagingRoot $Product
    New-Item -ItemType Directory -Force -Path (Join-Path $Stage 'Addons'), (Join-Path $Stage 'keys'), (Join-Path $Stage 'Settings') | Out-Null

    Copy-Item -LiteralPath (Join-Path $BuildRoot "Addons\$Product.pbo") -Destination (Join-Path $Stage "Addons\$Product.pbo") -Force
    Copy-Item -LiteralPath (Join-Path $BuildRoot "Addons\$Product.pbo.DeutschZ.bisign") -Destination (Join-Path $Stage "Addons\$Product.pbo.DeutschZ.bisign") -Force
    Copy-Item -LiteralPath (Join-Path $BuildRoot 'keys\DeutschZ.bikey') -Destination (Join-Path $Stage 'keys\DeutschZ.bikey') -Force
    Copy-Item -LiteralPath (Join-Path $SourceRoot 'mod.cpp') -Destination (Join-Path $Stage 'mod.cpp') -Force
    Copy-Item -LiteralPath (Join-Path $SourceRoot 'meta.cpp') -Destination (Join-Path $Stage 'meta.cpp') -Force
    Copy-Item -LiteralPath $SettingsSource -Destination (Join-Path $Stage "Settings\$SettingsName") -Force
    Copy-Item -LiteralPath $TestGuideSource -Destination (Join-Path $Stage 'TESTANLEITUNG.md') -Force
    Copy-Item -LiteralPath (Join-Path $BuildRoot 'preflight-report.txt') -Destination (Join-Path $Stage 'preflight-report.txt') -Force

    $Required = @(
        "Addons\$Product.pbo",
        "Addons\$Product.pbo.DeutschZ.bisign",
        'keys\DeutschZ.bikey',
        'mod.cpp',
        'meta.cpp',
        "Settings\$SettingsName",
        'TESTANLEITUNG.md',
        'preflight-report.txt'
    )
    foreach ($Relative in $Required) {
        if (-not (Test-Path -LiteralPath (Join-Path $Stage $Relative))) {
            throw "Staging validation failed for ${Product}: $Relative"
        }
    }

    $Target = Join-Path $OutputRoot "@$Product"
    if (-not $Target.StartsWith($OutputRoot + '\', [System.StringComparison]::OrdinalIgnoreCase)) {
        throw "Unsafe target: $Target"
    }

    if (Test-Path -LiteralPath $Target) {
        $OldTarget = Join-Path $OldRoot "@$Product"
        Move-Item -LiteralPath $Target -Destination $OldTarget
    }

    Move-Item -LiteralPath $Stage -Destination $Target
    Write-Output "SYNCED $Product -> $Target"
}

if ($Only -eq 'All' -or $Only -eq 'MenuZ') {
    Copy-Product `
        -Product 'DeutschZ_MenuMusic_V2_Test' `
        -BuildRoot 'C:\Users\patsl\DayZProjects\build\@DeutschZ_MenuMusic_V2_Test' `
        -SourceRoot 'E:\DeutschZ\DeutschZ-ModZ\src\DeutschZ_MenuMusic_V2_Test' `
        -SettingsSource 'E:\DeutschZ\DeutschZ-ModZ\src\DeutschZ_MenuMusic_V2_Test\data\settings\MenuZSettings.example.json' `
        -SettingsName 'MenuZSettings.json' `
        -TestGuideSource 'E:\DeutschZ\DeutschZ-ModZ\src\DeutschZ_MenuMusic_V2_Test\TESTANLEITUNG.md'
}

if ($Only -eq 'All' -or $Only -eq 'UiAdminZ') {
    Copy-Product `
        -Product 'DeutschZ_UiAdminZ' `
        -BuildRoot 'C:\Users\patsl\DayZProjects\build\@DeutschZ_UiAdminZ' `
        -SourceRoot 'E:\DeutschZ\DeutschZ-ModZ\src\DeutschZ_UiAdminZ' `
        -SettingsSource 'E:\DeutschZ\DeutschZ-ModZ\src\DeutschZ_UiAdminZ\example-settings\UiAdminZSettings.json' `
        -SettingsName 'UiAdminZSettings.json' `
        -TestGuideSource 'E:\DeutschZ\DeutschZ-ModZ\src\DeutschZ_UiAdminZ\docs\TESTANLEITUNG.md'
}

if (Test-Path -LiteralPath $StagingRoot) {
    Remove-Item -LiteralPath $StagingRoot -Recurse -Force
}
if (Test-Path -LiteralPath $OldRoot) {
    Remove-Item -LiteralPath $OldRoot -Recurse -Force
}
