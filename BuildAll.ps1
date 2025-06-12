param([string]$UE="C:\Program Files\Epic Games\UE_5.4")

if (-not (Test-Path "$UE\Engine\Build\BatchFiles\RunUAT.bat")) {
    throw "RunUAT.bat not found under $UE"
}

& "$UE\Engine\Build\BatchFiles\RunUAT.bat" BuildPlugin `
   -Plugin="$PSScriptRoot\Plugins\SquadAI\SquadAI.uplugin" `
   -Package="$PSScriptRoot\Packages\SquadAI" `
   -TargetPlatforms=Win64

