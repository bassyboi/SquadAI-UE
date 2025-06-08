#include "SquadCharacter.h"
#include "Components/SphereComponent.h"

ASquadCharacter::ASquadCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    CoverDebugSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CoverDebugSphere"));
    CoverDebugSphere->SetupAttachment(RootComponent);
    CoverDebugSphere->SetSphereRadius(30.f);
    CoverDebugSphere->SetHiddenInGame(false);

    MaxHealth = 100.f;
    CurrentHealth = MaxHealth;
}

void ASquadCharacter::BeginPlay()
{
    Super::BeginPlay();
    SetupHealth();
}

void ASquadCharacter::SetupHealth()
{
    CurrentHealth = MaxHealth;
}

float ASquadCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    CurrentHealth -= ActualDamage;
    if (CurrentHealth <= 0.f)
    {
        OnHealthZero();
    }
    return ActualDamage;
}

void ASquadCharacter::OnHealthZero()
{
    Destroy();
}
