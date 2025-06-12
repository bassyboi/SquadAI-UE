#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SquadSubsystem.generated.h"

class ASquadAIController;

/** Simple world subsystem providing unique squad IDs. */
UCLASS()
class SQUADAI_API USquadSubsystem : public UWorldSubsystem
{
GENERATED_BODY()
public:
int32 RegisterSquadMember(ASquadAIController* Controller);

private:
int32 NextSquadId = 0;
};
