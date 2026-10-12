$ErrorActionPreference = 'Stop'
$repo = Split-Path $PSScriptRoot
$dayzTools = 'C:\Program Files (x86)\Steam\steamapps\common\DayZ Tools'
$package = Join-Path $repo 'build\@Schwanz'
$env:TEMP = Join-Path $repo 'build\temp'
$env:TMP = $env:TEMP
New-Item -ItemType Directory -Force $env:TEMP | Out-Null
New-Item -ItemType Directory -Force "$package\addons", "$package\keys", "$package\WorldMemoriez\addons", "$package\WorldMemoriez\keys" | Out-Null
foreach ($module in @('deutschz_eventschedulerz','deutschz_world_memoriez','deutschz_ammostackz')) {
 $source = Join-Path $repo "mods\$module"
 & "$dayzTools\Bin\CfgConvert\CfgConvert.exe" -test "$source\config.cpp"
 if ($LASTEXITCODE -ne 0) {throw "CfgConvert failed: $module"}
 $destination = "$package\addons"
 if ($module -eq 'deutschz_world_memoriez') {$destination = "$package\WorldMemoriez\addons"}
 & "$dayzTools\Bin\AddonBuilder\AddonBuilder.exe" $source $destination -packonly "-prefix=$module" "-toolsDirectory=$dayzTools" *> (Join-Path $repo "build\$module.build.log")
 if ($LASTEXITCODE -ne 0) {throw "AddonBuilder failed: $module"}
 if (!(Test-Path -LiteralPath "$destination\$module.pbo")) {throw "AddonBuilder produced no PBO: $module"}
 if (Select-String -LiteralPath (Join-Path $repo "build\$module.build.log") -Pattern '\[FATAL\]|\[ERROR\]' -Quiet) {throw "AddonBuilder logged a build error: $module"}
}
Copy-Item -LiteralPath "$dayzTools\Bin\DsUtils\DZ_Neu.bikey" -Destination "$package\keys\DZ_Neu.bikey" -Force
Copy-Item -LiteralPath "$dayzTools\Bin\DsUtils\DZ_Neu.bikey" -Destination "$package\WorldMemoriez\keys\DZ_Neu.bikey" -Force
'PBO build complete. Script compilation still requires the operator server start.'
