$ErrorActionPreference = 'Stop'
$repo = Split-Path $PSScriptRoot
$dayzTools = 'C:\Program Files (x86)\Steam\steamapps\common\DayZ Tools'
$destination = Join-Path $repo 'build\@Schwanz\Issue14\addons'
New-Item -ItemType Directory -Force $destination, "$repo\build\@Schwanz\Issue14\keys" | Out-Null
foreach ($module in @('deutschz_carraidez','deutschz_witnessz','deutschz_adaptive_camouflage')) {
 $source = Join-Path $repo "mods\$module"
 & "$dayzTools\Bin\CfgConvert\CfgConvert.exe" -test "$source\config.cpp"
 if ($LASTEXITCODE -ne 0) {throw "Invalid configuration: $module"}
 $log = Join-Path $repo "build\$module.build.log"
 & "$dayzTools\Bin\AddonBuilder\AddonBuilder.exe" $source $destination -packonly "-prefix=$module" "-toolsDirectory=$dayzTools" *> $log
 if ($LASTEXITCODE -ne 0 -or !(Test-Path -LiteralPath "$destination\$module.pbo") -or (Select-String -LiteralPath $log -Pattern '\[FATAL\]|\[ERROR\]' -Quiet)) {throw "PBO build failed: $module"}
}
Copy-Item -LiteralPath "$dayzTools\Bin\DsUtils\DZ_Neu.bikey" -Destination "$repo\build\@Schwanz\Issue14\keys\DZ_Neu.bikey" -Force
'Issue14 PBOs built. DayZ script compilation requires the operator testserver start.'
