// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "PhysicsProjectile.generated.h"

/**
 *
 */
UCLASS()
class TANKOGEDDON_API APhysicsProjectile : public AProjectile
{
	GENERATED_BODY()

public:
	APhysicsProjectile();

	virtual void Start() override;
	virtual void Stop() override;
	virtual void Tick(float DeltaSeconds) override;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	//	float ExplosionRange = 1000.f;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	//	float ExplosionImpulse = 100.f;

protected:
	//virtual void OnMeshHit(class UPrimitiveComponent* HittedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& SweepResult) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UPhysicsMovementComponent* MovementComponent;
};