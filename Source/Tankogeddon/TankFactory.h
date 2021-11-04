// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Damageable.h"
#include "TankFactory.generated.h"

UCLASS()
class TANKOGEDDON_API ATankFactory : public AActor, public IDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATankFactory();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* BuildingMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UArrowComponent* TankSpawnPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UBoxComponent* HitCollider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UHealthComponent* HealthComponent;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
		TSubclassOf<class ATankPawn> SpawnTankClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn tanks params")
		float SpawnTankRate = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
		TArray<class ATargetPoint*> TankWayPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal Params")
		class AMapLoader* MapLoader;

public:

	virtual void TakeDamage(const FDamageData& DamageData) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	void SpawnNewTank();

	UFUNCTION()
		void Die();

	UFUNCTION()
		void DamageTaked(float DamageValue);

private:
	FTimerHandle SpawnTankTimerHandle;
};