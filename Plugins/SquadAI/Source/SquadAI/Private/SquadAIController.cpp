#include "SquadAIController.h"
#include "BehaviorTree/BehaviorTree.h"

ASquadAIController::ASquadAIController()
{
}

void ASquadAIController::OnUnderFire(const FAIStimulus& Stimulus)
{
    // TODO: React to gunfire stimulus
}

void ASquadAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    if (SquadBehaviour)
    {
        RunBehaviorTree(SquadBehaviour);
    }
}

void ASquadAIController::FindAndMoveToCover(const FVector& ThreatLocation)
{
    // TODO: Implement EQS query to select best cover location
}
