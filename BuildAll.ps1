param([string]$UE="C:\Program Files\Epic Games\UE_5.4")
& "$UE\Engine\Build\BatchFiles\RunUAT.bat" BuildPlugin `
   -Plugin="$PSScriptRoot\Plugins\SquadAI\SquadAI.uplugin" `
   -Package="$PSScriptRoot\Packages\SquadAI" -TargetPlatforms=Win64,Lin64

