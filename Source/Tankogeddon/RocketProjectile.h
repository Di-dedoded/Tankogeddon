// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "RocketProjectile.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ARocketProjectile : public AProjectile
{
	GENERATED_BODY()
	
public:
	ARocketProjectile();

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	float EngineThrust = 1.f;
};
