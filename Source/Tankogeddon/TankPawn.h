// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameStructs.h"
#include "Damageable.h"
#include "Scoreable.h"
#include "TankPawn.generated.h"

UCLASS()
class TANKOGEDDON_API ATankPawn : public APawn, public IDamageable, public IScoreable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankPawn();

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
    class UStaticMeshComponent* BodyMesh;
    
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
    class UStaticMeshComponent* TurretMesh;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    class USpringArmComponent* SpringArm;
    
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    class UCameraComponent* Camera;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    class UArrowComponent* CannonSpawnPoint;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    class UHealthComponent* HealthComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    class UBoxComponent* HitCollider;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
    float MoveSpeed = 100.f;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
    float RotationSpeed = 100.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
    float MovementSmootheness = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
    float RotationSmootheness = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
    float TurretRotationSmootheness = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
    TSubclassOf<class ACannon> DefaultCannonClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Scores")
    int32 DestructionScores = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params", Meta = (MakeEditWidget = true))
    TArray<class ATargetPoint*> PatrollingPoints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params")
    float MovementAccuracy = 50.f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UFUNCTION(BlueprintNativeEvent, Category = "Health")
    void DamageTaked(float Damage);

    UFUNCTION(BlueprintNativeEvent, Category = "Health")
    void Die();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void MoveForward(float InAxisValue);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void RotateRight(float InAxisValue);

    UFUNCTION(BlueprintCallable, Category = "Turret")
    void SetTurretTargetPosition(const FVector& TargetPosition);

    UFUNCTION(BlueprintCallable, Category = "Turret")
    void SetTurretRotationAxis(float AxisValue);
    
    UFUNCTION(BlueprintCallable, Category = "Turret")
    void Fire();

    UFUNCTION(BlueprintCallable, Category = "Turret")
    void FireSpecial();

    UFUNCTION(BlueprintCallable, Category = "Turret")
    void SetupCannon(TSubclassOf<class ACannon> InCannonClass);

    UFUNCTION(BlueprintCallable, Category = "Turret")
    void CycleCannon();

    UFUNCTION(BlueprintPure, Category = "Turret")
    class ACannon* GetActiveCannon() const;

    UFUNCTION(BlueprintPure, Category = "Turret")
    FVector GetTurretForwardVector();

    virtual void TakeDamage(const FDamageData& DamageData) override;
    int32 GetScores() const override;

    UFUNCTION(BlueprintCallable, Category = "AI|Move params")
    void SetPatrollingPoints(const TArray<class ATargetPoint*>& InPoints)
    {
        PatrollingPoints = InPoints;
    }

    UFUNCTION(BlueprintPure, Category = "AI|Move params")
    const TArray<class ATargetPoint*>& GetPatrollingPoints() const
    { 
        return PatrollingPoints;
    }

    UFUNCTION(BlueprintPure, Category = "AI|Move params")
    float GetMovementAccuracy() 
    { 
        return MovementAccuracy; 
    }    


//protected:

private:
    UPROPERTY()
    class ACannon* ActiveCannon = nullptr;

    UPROPERTY()
    ACannon* InactiveCannon = nullptr;

    float CurrentMoveForwardAxis = 0.f;
    float TargetMoveForwardAxis = 0.f;
    float CurrentRotateRightAxis = 0.f;
    float TargetRotateRightAxis = 0.f;

    FVector TurretTargetPosition;
    FVector TurretTargetDirection;
    bool bIsTurretTargetSet = false;
    float TurretRotationAxis = 0.f;
};
