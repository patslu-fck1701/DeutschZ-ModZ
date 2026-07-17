param(
    [string]$Root = 'E:\DeutschZ\DeutschZ-ModZ\src\DeutschZ_UiAdminZ\gui\admin_pack'
)

$resolved = [System.IO.Path]::GetFullPath($Root)
$expected = [System.IO.Path]::GetFullPath('E:\DeutschZ\DeutschZ-ModZ\src\DeutschZ_UiAdminZ\gui\admin_pack')
if ($resolved -ne $expected) {
    throw "Unerwarteter Cleanup-Pfad: $resolved"
}

$pngs = Get-ChildItem -LiteralPath $resolved -Recurse -File -Filter '*.png'
foreach ($png in $pngs) {
    [System.IO.File]::Delete($png.FullName)
}
Write-Host "Entfernte generierte Zwischen-PNGs: $($pngs.Count)"
