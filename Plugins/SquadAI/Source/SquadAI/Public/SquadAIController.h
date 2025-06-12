#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameplayTagContainer.h"
#include "Perception/AIPerceptionTypes.h"
#include "SquadAIController.generated.h"

/**
 *  ASquadAIController
 *  ───────────────────
 *  • Owns the squad-wide blackboard (shared via tag "SquadId").
 *  • Drives formation, suppression, and role-based ability usage.
 *  • Delegates expensive ops (cover search, EQS) to BT services/tasks.
 */
UCLASS()
class SQUADAI_API ASquadAIController : public AAIController
{
GENERATED_BODY()

public:
ASquadAIController();

/** Returns the squad’s numeric ID (same for every pawn in a squad). */
int32 GetSquadId() const { return SquadId; }

/** Push incoming damage for suppression tracking. */
void NotifyIncomingFire(const FVector& ShotDir);

protected:
virtual void OnPossess(APawn* InPawn) override;
virtual void Tick(float DeltaTime) override;

/** Cached blackboard keys (avoids FName lookup every frame). */
struct FBBKeys
{
uint8 EnemyActor;
uint8 CoverLocation;
uint8 FormationSlot;
uint8 SuppressionValue;
} Keys;

private:
/* ---------- CONFIG (set in CDO / BP child) ---------- */
UPROPERTY(EditDefaultsOnly, Category="Squad|Behaviour")
class UBehaviorTree* DefaultBT = nullptr;

UPROPERTY(EditDefaultsOnly, Category="Squad|Formation")
float FormationRecalcInterval = 1.f;

UPROPERTY(EditDefaultsOnly, Category="Squad|Suppression")
float SuppressionDecayPerSecond = 15.f;

/* ---------- STATE ---------- */
int32 SquadId = INDEX_NONE;
float NextFormationUpdateTime = 0.f;

/* ---------- HELPERS ---------- */
void UpdateFormation(float Now);
void DecaySuppression(float DeltaTime);
};
