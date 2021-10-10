// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupInputComponent() override;
private:
	virtual void MoveForward(float InAxisValue);
	virtual void MoveRight(float InAxisValue);

	UPROPERTY()
	class ATankPawn* TankPawn;
};
 