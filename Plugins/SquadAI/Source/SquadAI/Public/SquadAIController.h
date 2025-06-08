#pragma once
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "TimerManager.h"
#include "SquadAIController.generated.h"

UCLASS()
class ASquadAIController : public AAIController
{
    GENERATED_BODY()

public:
    ASquadAIController();

protected:
    virtual void OnPossess(APawn* InPawn) override;
    UFUNCTION()
    void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

    void FindAndMoveToCover(const FVector& ThreatLocation);

private:
    void HandleUnderFire(const FAIStimulus& Stimulus);
    void ClearUnderFire();

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UBehaviorTree* SquadBehaviour;

    UPROPERTY(VisibleAnywhere, Category = "AI")
    UAIPerceptionComponent* PerceptionComp;

    FTimerHandle ClearUnderFireHandle;

    FName BB_ThreatLoc = "ThreatLocation";
    FName BB_IsUnderFire = "IsUnderFire";
};
