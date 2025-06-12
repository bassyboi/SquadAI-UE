#include "SquadAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SquadSubsystem.h"
#include "DrawDebugHelpers.h"

ASquadAIController::ASquadAIController()
{
    PrimaryActorTick.bCanEverTick = true;
    bAttachToPawn = true;
}

void ASquadAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (USquadSubsystem* Sub = GetWorld()->GetSubsystem<USquadSubsystem>())
    {
        SquadId = Sub->RegisterSquadMember(this);
    }

    if (DefaultBT) RunBehaviorTree(DefaultBT);

    if (UBlackboardComponent* BB = GetBlackboardComponent())
    {
        Keys.EnemyActor       = BB->GetKeyID("EnemyActor");
        Keys.CoverLocation    = BB->GetKeyID("CoverLocation");
        Keys.FormationSlot    = BB->GetKeyID("FormationSlot");
        Keys.SuppressionValue = BB->GetKeyID("SuppressionValue");
    }
}

void ASquadAIController::Tick(float Delta)
{
    Super::Tick(Delta);

    const float Now = GetWorld()->GetTimeSeconds();
    if (Now >= NextFormationUpdateTime)
    {
        UpdateFormation(Now);
        NextFormationUpdateTime = Now + FormationRecalcInterval;
    }

    DecaySuppression(Delta);
}

void ASquadAIController::NotifyIncomingFire(const FVector& /*ShotDir*/)
{
    if (UBlackboardComponent* BB = GetBlackboardComponent())
    {
        const float Old = BB->GetValueAsFloat(Keys.SuppressionValue);
        BB->SetValueAsFloat(Keys.SuppressionValue, FMath::Min(100.f, Old + 30.f));
    }
}

void ASquadAIController::DecaySuppression(float Delta)
{
    if (UBlackboardComponent* BB = GetBlackboardComponent())
    {
        const float Old = BB->GetValueAsFloat(Keys.SuppressionValue);
        if (Old > 0.f)
        {
            BB->SetValueAsFloat(Keys.SuppressionValue,
                                FMath::Max(0.f, Old - SuppressionDecayPerSecond * Delta));
        }
    }
}

void ASquadAIController::UpdateFormation(float /*Now*/)
{
    if (APawn* MyPawn = GetPawn())
    {
        if (UBlackboardComponent* BB = GetBlackboardComponent())
        {
            const int32 Slot = BB->GetValueAsInt(Keys.FormationSlot);
            static const FVector Offsets[4] = {
                {0.f,   0.f, 0.f},
                {0.f, -300.f,0.f},
                {0.f,  300.f,0.f},
                {-300.f,0.f, 0.f}
            };
            const FVector Dest = MyPawn->GetActorLocation() + Offsets[Slot].RotateAngleAxis(
                MyPawn->GetActorRotation().Yaw, FVector::UpVector);

            MoveToLocation(Dest, 50.f);
#if !(UE_BUILD_SHIPPING)
            DrawDebugSphere(GetWorld(), Dest, 25.f, 8, FColor::Cyan, false, FormationRecalcInterval);
#endif
        }
    }
}
