// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InventoryCharacter.generated.h"

class UInventoryComponent;
class UInventoryManagerComponent;

UCLASS()
class TANKOGEDDON_API AInventoryCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AInventoryCharacter();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	UInventoryComponent* LocalInventory;

	UPROPERTY(EditDefaultsOnly)
	UInventoryManagerComponent* InventoryManager;

};
