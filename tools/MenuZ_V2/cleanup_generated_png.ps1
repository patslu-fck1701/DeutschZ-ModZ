$ErrorActionPreference = 'Stop'

$Root = 'E:\DeutschZ\DeutschZ-ModZ\src\DeutschZ_MenuMusic_V2_Test\gui\v2_kit'
$ResolvedRoot = (Resolve-Path -LiteralPath $Root).Path
$ExpectedRoot = 'E:\DeutschZ\DeutschZ-ModZ\src\DeutschZ_MenuMusic_V2_Test\gui\v2_kit'

if ($ResolvedRoot -ne $ExpectedRoot) {
    throw "Unexpected generated asset root: $ResolvedRoot"
}

$Files = Get-ChildItem -LiteralPath $ResolvedRoot -Recurse -File -Filter '*.png'
foreach ($File in $Files) {
    if (-not $File.FullName.StartsWith($ResolvedRoot + '\', [System.StringComparison]::OrdinalIgnoreCase)) {
        throw "Refusing to remove file outside generated asset root: $($File.FullName)"
    }

    Remove-Item -LiteralPath $File.FullName -Force
}

Write-Output "Removed generated PNG intermediates: $($Files.Count)"
