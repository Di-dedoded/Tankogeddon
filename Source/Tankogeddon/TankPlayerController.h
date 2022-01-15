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
	virtual void Tick(float DeltaSeconds) override;

    UPROPERTY()
    int32 Scores;

	FSimpleMulticastDelegate OnMouseButtonUp;

	void OnLeftMouseButton();
	
private:
	void MoveForward(float InAxisValue);
	void RotateRight(float InAxisValue);
	void Fire();
	void FireSpecial();
	void CycleCannon();

    UFUNCTION(exec)
    void DumpActorPoolSubsystemStats();

	UPROPERTY()
	class ATankPawn* TankPawn;

    FVector2D LastFrameMousePosition;
    bool bIsControllingFromMouse = false;
};
