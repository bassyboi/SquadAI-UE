#pragma once
#include "GameFramework/Character.h"
#include "SquadCharacter.generated.h"

UCLASS()
class ASquadCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ASquadCharacter();

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
    virtual void BeginPlay() override;

    void SetupHealth();

    UFUNCTION()
    void OnHealthZero();

    UPROPERTY(EditAnywhere, Category="Squad")
    float MaxHealth;

    UPROPERTY(VisibleAnywhere, Category="Squad")
    float CurrentHealth;

    UPROPERTY(VisibleAnywhere)
    class USphereComponent* CoverDebugSphere;
};
