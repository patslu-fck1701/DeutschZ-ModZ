param(
    [string]$OutputRoot = 'E:\DeutschZ\DeutschZServer',
    [string]$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot '..\..')).Path,
    [string]$BuildRoot = (Join-Path $RepoRoot 'build\@DeutschZ_KotHZ_Free'),
    [string]$KeyPath = 'C:\Users\patsl\DayZProjects\keys\DeutschZ.bikey'
)

$ErrorActionPreference = 'Stop'
$stagingRoot = Join-Path $env:TEMP ("DeutschZ_publish_" + [guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Path $stagingRoot -Force | Out-Null

function Assert-OutputTarget([string]$path) {
    $expectedParent = [IO.Path]::GetFullPath($OutputRoot).TrimEnd('\')
    $actualParent = [IO.Path]::GetFullPath((Split-Path $path -Parent)).TrimEnd('\')
    if ($actualParent -ne $expectedParent) { throw "Unsafe output target outside OutputRoot: $path" }
}

function Publish-Directory([string]$stagePath, [string]$targetPath) {
    Assert-OutputTarget $targetPath
    if (-not (Test-Path -LiteralPath $stagePath -PathType Container)) { throw "Missing staged product: $stagePath" }
    $backupPath = Join-Path $stagingRoot ("old_" + (Split-Path $targetPath -Leaf))
    if (Test-Path -LiteralPath $targetPath) { Move-Item -LiteralPath $targetPath -Destination $backupPath }
    try {
        Move-Item -LiteralPath $stagePath -Destination $targetPath
        if (-not (Test-Path -LiteralPath $targetPath -PathType Container)) { throw "Published target not found: $targetPath" }
        if (Test-Path -LiteralPath $backupPath) { Remove-Item -LiteralPath $backupPath -Recurse -Force }
    } catch {
        if (Test-Path -LiteralPath $targetPath) { Remove-Item -LiteralPath $targetPath -Recurse -Force }
        if (Test-Path -LiteralPath $backupPath) { Move-Item -LiteralPath $backupPath -Destination $targetPath }
        throw
    }
}

try {
    $kothStage = Join-Path $stagingRoot '@DeutschZ_KotHZ_Free'
    New-Item -ItemType Directory -Path (Join-Path $kothStage 'Addons'), (Join-Path $kothStage 'keys'), (Join-Path $kothStage 'Settings') -Force | Out-Null
    Copy-Item -LiteralPath (Join-Path $BuildRoot 'Addons\DeutschZ_KotHZ_Free.pbo') -Destination (Join-Path $kothStage 'Addons')
    Copy-Item -LiteralPath (Join-Path $BuildRoot 'Addons\DeutschZ_KotHZ_Free.pbo.DeutschZ.bisign') -Destination (Join-Path $kothStage 'Addons')
    Copy-Item -LiteralPath $KeyPath -Destination (Join-Path $kothStage 'keys\DeutschZ.bikey')
    Copy-Item -LiteralPath (Join-Path $RepoRoot 'src\DeutschZ_KotHZ_Free\mod.cpp') -Destination $kothStage
    Copy-Item -LiteralPath (Join-Path $RepoRoot 'src\DeutschZ_KotHZ_Free\meta.cpp') -Destination $kothStage
    Copy-Item -LiteralPath (Join-Path $RepoRoot 'src\DeutschZ_KotHZ_Free\docs\TESTANLEITUNG.md') -Destination $kothStage
    Copy-Item -LiteralPath (Join-Path $RepoRoot 'src\DeutschZ_KotHZ_Free\example-settings\KotHZSettings.json') -Destination (Join-Path $kothStage 'Settings\KotHZSettings.json')
    Copy-Item -LiteralPath (Join-Path $RepoRoot 'src\DeutschZ_KotHZ_Free\example-settings\KotHZLocations.json') -Destination (Join-Path $kothStage 'Settings\KotHZLocations.json')
    if (@(Get-ChildItem -LiteralPath (Join-Path $kothStage 'Addons') -Filter '*.pbo').Count -ne 1) { throw 'KotHZ stage PBO count mismatch.' }
    Publish-Directory $kothStage (Join-Path $OutputRoot '@DeutschZ_KotHZ_Free')

    $coreStage = Join-Path $stagingRoot '@DeutschZ_only_core'
    New-Item -ItemType Directory -Path (Join-Path $coreStage 'Addons'), (Join-Path $coreStage 'keys') -Force | Out-Null
    $products = @(
        @{ Root = (Join-Path $OutputRoot '@DeutschZ_MenuMusic_V2_Test'); Name = 'DeutschZ_MenuMusic_V2_Test' },
        @{ Root = (Join-Path $OutputRoot '@DeutschZ_KotHZ_Free'); Name = 'DeutschZ_KotHZ_Free' },
        @{ Root = (Join-Path $OutputRoot '@DeutschZ_UiAdminZ'); Name = 'DeutschZ_UiAdminZ' }
    )
    foreach ($product in $products) {
        Copy-Item -LiteralPath (Join-Path $product.Root ("Addons\" + $product.Name + '.pbo')) -Destination (Join-Path $coreStage 'Addons')
        Copy-Item -LiteralPath (Join-Path $product.Root ("Addons\" + $product.Name + '.pbo.DeutschZ.bisign')) -Destination (Join-Path $coreStage 'Addons')
    }
    Copy-Item -LiteralPath $KeyPath -Destination (Join-Path $coreStage 'keys\DeutschZ.bikey')
    Copy-Item -LiteralPath (Join-Path $RepoRoot 'packaging\DeutschZ_only_core\mod.cpp') -Destination $coreStage
    Copy-Item -LiteralPath (Join-Path $RepoRoot 'packaging\DeutschZ_only_core\meta.cpp') -Destination $coreStage
    Copy-Item -LiteralPath (Join-Path $RepoRoot 'packaging\DeutschZ_only_core\TESTANLEITUNG.md') -Destination $coreStage
    $pboNames = @(Get-ChildItem -LiteralPath (Join-Path $coreStage 'Addons') -Filter '*.pbo' | ForEach-Object Name | Sort-Object)
    $expectedPbos = @('DeutschZ_KotHZ_Free.pbo','DeutschZ_MenuMusic_V2_Test.pbo','DeutschZ_UiAdminZ.pbo') | Sort-Object
    if (Compare-Object $pboNames $expectedPbos) { throw "only_core PBO set mismatch: $($pboNames -join ', ')" }
    if (@(Get-ChildItem -LiteralPath (Join-Path $coreStage 'Addons') -Filter '*.bisign').Count -ne 3) { throw 'only_core BISIGN count mismatch.' }
    if (@(Get-ChildItem -LiteralPath (Join-Path $coreStage 'keys') -Filter '*.bikey').Count -ne 1) { throw 'only_core BIKEY count mismatch.' }
    Publish-Directory $coreStage (Join-Path $OutputRoot '@DeutschZ_only_core')

    Get-ChildItem -LiteralPath (Join-Path $OutputRoot '@DeutschZ_only_core') -Recurse -File | Sort-Object FullName | Select-Object FullName, Length
} finally {
    if (Test-Path -LiteralPath $stagingRoot) { Remove-Item -LiteralPath $stagingRoot -Recurse -Force }
}
