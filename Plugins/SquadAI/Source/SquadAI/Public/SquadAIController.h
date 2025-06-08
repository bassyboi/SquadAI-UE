#pragma once
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "SquadAIController.generated.h"

UCLASS()
class ASquadAIController : public AAIController
{
    GENERATED_BODY()

public:
    ASquadAIController();

    UFUNCTION()
    void OnUnderFire(const FAIStimulus& Stimulus);

protected:
    virtual void OnPossess(APawn* InPawn) override;

private:
    void FindAndMoveToCover(const FVector& ThreatLocation);

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UBehaviorTree* SquadBehaviour;

    FName BB_ThreatLoc = "ThreatLocation";
    FName BB_IsUnderFire = "IsUnderFire";
};
