# SquadAI-UE

This repository contains a plugin skeleton for squad-based AI in Unreal Engine 5. It demonstrates a typical layout for distributing AI logic as a plugin that can be dropped into any project.

## Overview
The plugin provides a custom `ASquadAIController` class intended to drive AI pawns using behaviour trees and EQS for dynamic cover selection and squad behaviour.

## Requirements
- Unreal Engine 5.4 (or compatible)
- Visual Studio 2022 or another compiler supported by UE5

## Quick Start
1. Clone the repository.
2. Generate project files using the Unreal Editor or `GenerateProjectFiles.bat`.
3. Open the project in your IDE and compile.
4. Launch the editor and enable the **SquadAI** plugin if necessary.

For a detailed blueprint on structuring a plugin repository with Git and GitHub, see [SETUP_GUIDE.md](SETUP_GUIDE.md).

## Building the Plugin

The repository includes a small PowerShell script, `BuildAll.ps1`, that wraps
Unreal's `RunUAT` automation tool. It packages the plugin so it can be dropped
into any project. The script targets only Windows by default and expects Unreal
Engine to be installed under `C:\Program Files\Epic Games\UE_5.4` (override
with the `-UE` parameter if needed).

```
PS> ./BuildAll.ps1
```

After the command finishes you will find the packaged plugin under
`Packages/SquadAI`. The generated archive contains a build for `Win64`.
