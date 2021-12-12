// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStructs.h"
#include "Cannon.generated.h"

UCLASS()
class TANKOGEDDON_API ACannon : public AActor
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    class UStaticMeshComponent* Mesh;
    
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    class UArrowComponent* ProjectileSpawnPoint;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    class UParticleSystemComponent* ShootEffect;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    class UAudioComponent* AudioEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
    float FireRate = 1.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "Type == ECannonType::FireTrace", EditConditionHides), Category = "Fire params")
    float FireRange = 1000.f;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "Type == ECannonType::FireTrace", EditConditionHides), Category = "Fire params")
    float FireDamage = 1.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
    bool bHasSpecialFire = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
    int32 MaxAmmo = 10;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = 1), Category = "Fire params")
    int32 NumShotsInSeries = 1;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "NumShotsInSeries > 1", EditConditionHides), Category = "Fire params")
    float SeriesLength = 1.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
    ECannonType Type = ECannonType::FireProjectile;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "Type == ECannonType::FireProjectile", EditConditionHides), Category = "Fire params")
    TSubclassOf<class AProjectile> ProjectileClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
    class UForceFeedbackEffect* ShootForceEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
    TSubclassOf<class UCameraShakeBase> ShootShake;

private:
    FTimerHandle ReloadTimerHandle;
    FTimerHandle SeriesTimerHandle;
    bool bIsReadyToFire = false;

public:
    ACannon();

    void Fire();
    void FireSpecial();

    bool IsReadyToFire();
    bool HasSpecialFire() const;
    void SetVisibility(bool bIsVisible);
    void AddAmmo(int32 InNumAmmo);

    bool IsBallistic() const;
    FVector GetCurrentBallisticTarget(float FloorAbsoluteHeight) const;
    // Returns false if can't do it
    bool SetDesiredBallisticTarget(const FVector& InTarget);

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

    void Reload();
    void Shot();

    int32 NumAmmo = 0;
    int32 ShotsLeft = 0;
};
