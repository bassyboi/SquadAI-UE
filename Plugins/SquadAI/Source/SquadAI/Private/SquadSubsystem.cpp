#include "SquadSubsystem.h"
#include "SquadAIController.h"

int32 USquadSubsystem::RegisterSquadMember(ASquadAIController* /*Controller*/)
{
    return NextSquadId++;
}
