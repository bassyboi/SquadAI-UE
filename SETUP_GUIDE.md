Below is a practical, step-by-step blueprint for turning your squad-AI work into a clean GitHub repository that you (and any collaborators) can pull straight into Unreal Engine 5. It covers repo layout, Git/LFS setup, C++ module scaffolding, and an optional GitHub Actions build so you can verify that the code still compiles on every push.

---

## 1 — Pick a delivery style: **Plugin vs. Full Project**

| When to choose | Result in UE | Folder to copy |
| --- | --- | --- |
| **Plugin (recommended)**<br> – You want to drop the AI into *any* project or distribute on the Marketplace later. | `YourProject/Plugins/SquadAI` | Just the plugin folder |
| **Full project**<br> – You’re building an entire game, not a reusable module. | Whole `.uproject` | Whole repo |

Below I assume the **plugin** route, but the same Git/GitHub advice applies to a full project.

---

## 2 — Create and prime the repo

1. **New repo → GitHub → “SquadAI-UE” (public or private).**
2. **Clone locally** and run the following helper script (or by hand):

```bash
# Initialise with common UE exclusions & LFS
git init
curl -L https://raw.githubusercontent.com/github/gitignore/main/UnrealEngine.gitignore -o .gitignore
git lfs install
git lfs track "*.uasset" "*.umap" "*.uproject" "*.png" "*.fbx"
```

3. **Add a basic README.md** describing:
   * Overview of the dynamic cover / squad logic.
   * Required UE version (→ commit SHA if you’re on Epic’s source build).
   * Quick-start instructions (clone, GenerateProjectFiles, compile).

4. Commit:
   ```bash
   git add .
   git commit -m "Initial commit – repo skeleton with UE gitignore + LFS"
   git push -u origin main
   ```

---

## 3 — Scaffold the C++ plugin

### 3.1 Generate the plugin in UE-Editor

```
Plugins ▸ New Plugin ▸ Gameplay Feature ▸ Name: SquadAI
```

UE will create `Plugins/SquadAI/SquadAI.uplugin` and a minimal `Source/SquadAI` module. Shut the editor, stage, and commit those files.

### 3.2 Flesh out the module

File: `Plugins/SquadAI/Source/SquadAI/SquadAI.Build.cs`

```csharp
public class SquadAI : ModuleRules
{
    public SquadAI(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core", "CoreUObject", "Engine",
            "AIModule",        // Behaviour Trees, EQS
            "GameplayTasks"    // MoveTo, Tasks
        });
    }
}
```

Example controller header (`SquadAIController.h`):

```cpp
#pragma once
#include "AIController.h"
#include "SquadAIController.generated.h"

UCLASS()
class ASquadAIController : public AAIController
{
    GENERATED_BODY()

public:
    ASquadAIController();

    /** Called by PerceptionComponent when gunfire sound stimuli occurs */
    UFUNCTION()
    void OnUnderFire(const FAIStimulus& Stimulus);

protected:
    virtual void OnPossess(APawn* InPawn) override;

private:
    void FindAndMoveToCover(const FVector& ThreatLocation);

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UBehaviorTree* SquadBehaviour;

    // BlackBoard keys cached as FNames
    FName BB_ThreatLoc       = "ThreatLocation";
    FName BB_IsUnderFire     = "IsUnderFire";
};
```

Add your Behaviour Tree (`BT_SquadLogic.uasset`) under `Content/SquadAI/AI`. Because it’s binary it will be stored via LFS.

---

## 4 — Repo structure snapshot

```
SquadAI-UE/
├─ .gitattributes          # LFS rules
├─ .gitignore
├─ README.md
├─ Plugins/
│  └─ SquadAI/
│     ├─ SquadAI.uplugin
│     ├─ Source/
│     │   └─ SquadAI/
│     │      ├─ Public/
│     │      │   ├─ SquadAIController.h
│     │      │   └─ ...
│     │      └─ Private/
│     │         ├─ SquadAIController.cpp
│     │         └─ ...
│     └─ Content/
│        └─ AI/
│           ├─ BT_SquadLogic.uasset
│           └─ BB_SquadBot.uasset
└─ Examples/
   └─ TestMap.umap
```

---

## 5 — Branch & workflow conventions

| Branch      | Purpose                                  | Protection rules            |
| ----------- | ---------------------------------------- | --------------------------- |
| `main`      | Always buildable, tagged for releases    | Require CI pass, PR reviews |
| `dev`       | Integration of multiple feature branches | Open testing                |
| `feature/*` | One task (e.g. `feature/cover-system`)   | Free to rebase              |

---

## 6 — Optional CI: GitHub Actions to verify Windows build

`.github/workflows/ue-plugin-ci.yml`

```yaml
name: Build-Plugin-Win64

on:
  push: { branches: [main, dev] }
  pull_request:

jobs:
  build:
    runs-on: windows-2022
    steps:
      - uses: actions/checkout@v4
        with:
          lfs: true

      - name: Setup MSVC
        uses: ilammy/msvc-dev-cmd@v1

      - name: Cache UE binaries
        uses: actions/cache@v4
        with:
          path: "C:/Program Files/Epic Games/UE_5.4"
          key: UE5.4-win64

      - name: Generate project files
        run: |
          & "C:/Program Files/Epic Games/UE_5.4/Engine/Build/BatchFiles/RunUAT.bat"             -ScriptsForProject=`"${{ github.workspace }}/DummyProject/Dummy.uproject`"             GenerateProjectFiles -projectfiles

      - name: Build plugin
        run: |
          & "C:/Program Files/Epic Games/UE_5.4/Engine/Build/BatchFiles/RunUAT.bat" BuildPlugin ^
             -Plugin="${{ github.workspace }}/Plugins/SquadAI/SquadAI.uplugin" ^
             -Package="${{ github.workspace }}/PluginPackages/SquadAI" ^
             -TargetPlatforms=Win64

      - name: Upload artifact
        uses: actions/upload-artifact@v4
        with:
          name: SquadAI-win64
          path: PluginPackages/SquadAI
```

*Why?*
Every push / PR tries a headless compile; you catch API breaks before they reach the editor.

---

## 7 — Pulling the plugin into UE5

```bash
git clone https://github.com/YourUser/SquadAI-UE.git
cd SquadAI-UE
<open UnrealEditor.exe>
```

UE detects the plugin, prompts for a recompile (if binaries absent), and you’re ready to drop the Behaviour Tree on any AI-controlled pawn.

---

## 8 — Next implementation milestones

| Milestone                 | Key tasks                                                                                     |
| ------------------------- | --------------------------------------------------------------------------------------------- |
| **Cover selection logic** | Implement EQS test that scores NavMesh points by distance & visibility to ThreatLocation      |
| **Squad cohesion**        | Add `USquadManagerSubsystem` that issues compact formation MoveTo commands when not in combat |
| **Network replication**   | Ensure decisions (e.g., selected cover) replicate via `RPC_ServerChooseCover`                 |
| **Stress test**           | GitHub Action build (Win64 only) + functional tests with Gauntlet                    |

---

### Need more depth?

* Want help writing the EQS tests or BT graph?
* Need to wire this repo into a larger monorepo containing maps, art, and audio?
* Curious about packaging the plugin for the Unreal Marketplace?

Just ask and we’ll zoom into that section. Happy coding!
