#include "SquadAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ASquadAIController::ASquadAIController()
{
    PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));
    SetPerceptionComponent(*PerceptionComp);
}

void ASquadAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
    for (AActor* Actor : UpdatedActors)
    {
        FAIStimulus Stimulus;
        if (PerceptionComp->GetActorsPerception(Actor, Stimulus))
        {
            if (Stimulus.Tag == FName("Gunfire") && Stimulus.WasSuccessfullySensed())
            {
                HandleUnderFire(Stimulus);
            }
        }
    }
}

void ASquadAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    if (SquadBehaviour)
    {
        RunBehaviorTree(SquadBehaviour);
    }
    if (PerceptionComp)
    {
        PerceptionComp->OnPerceptionUpdated.AddDynamic(this, &ASquadAIController::OnPerceptionUpdated);
    }
}

void ASquadAIController::FindAndMoveToCover(const FVector& ThreatLocation)
{
    // TODO: Implement EQS query to select best cover location
}

void ASquadAIController::HandleUnderFire(const FAIStimulus& Stimulus)
{
    if (UBlackboardComponent* BB = GetBlackboardComponent())
    {
        BB->SetValueAsVector(BB_ThreatLoc, Stimulus.StimulusLocation);
        BB->SetValueAsBool(BB_IsUnderFire, true);
    }
    FindAndMoveToCover(Stimulus.StimulusLocation);
    GetWorldTimerManager().SetTimerForNextTick([this]()
    {
        GetWorldTimerManager().SetTimer(ClearUnderFireHandle, this, &ASquadAIController::ClearUnderFire, 3.0f, false);
    });
}

void ASquadAIController::ClearUnderFire()
{
    if (UBlackboardComponent* BB = GetBlackboardComponent())
    {
        BB->SetValueAsBool(BB_IsUnderFire, false);
    }
}
