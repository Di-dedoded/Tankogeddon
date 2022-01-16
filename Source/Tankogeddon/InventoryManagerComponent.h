// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryData.h"
#include "Components/ActorComponent.h"
#include "InventoryManagerComponent.generated.h"

class UInventoryComponent;
class UInventoryWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKOGEDDON_API UInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UInventoryManagerComponent();

	void Init(UInventoryComponent* InInventoryComponent);

	const FInventoryItemInfo* GetItemData(const FName& InID) const;

protected:

	UPROPERTY()
	UInventoryComponent* LocalInventoryComponent;

	UPROPERTY(EditAnywhere)
	UDataTable* ItemsData;

	UPROPERTY(EditAnywhere)
	int32 MinInventorySize = 10;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	UPROPERTY()
	UInventoryWidget* InventoryWidget;

	void OnItemDropFunc(UInventoryCellWidget* From, UInventoryCellWidget* To);

};
