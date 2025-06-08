# Blueprint Prompts

These prompts can be fed into the Blueprint generator to create helper functions.

## HandleUnderFire (AIController)
```
CreateFunction HandleUnderFire
Parent BP_SquadAIController
Node1 Set IsUnderFire=true
Node2 Set ThreatLocation=InStimulus.Location
Node3 Call FindCoverLocation Source=Self ThreatLoc=ThreatLocation -> CoverLoc
Node4 AI_MoveTo Pawn=ControlledPawn Destination=CoverLoc AcceptanceRadius=60
Node5 RetriggerableDelay 3
Node6 Set IsUnderFire=false
```

## InitializeAI (Character)
```
CreateFunction InitializeAI
Parent BP_SquadCharacter
Node1 Call SpawnDefaultController
Node2 Call SetupHealth
Node3 Call RegisterWithSquadManager Self
Node4 PrintString "Init OK"
```

## FindCoverLocation (AIController, pure)
```
CreateFunction FindCoverLocation
Parent BP_SquadAIController
Node1 EQS_RunQuery Template=EQS_FindCover Querier=ControlledPawn ThreatLoc=ThreatLocation
Node2 ReturnResult ResultLocation
```
