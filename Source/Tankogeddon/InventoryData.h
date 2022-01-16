// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Engine/DataTable.h"
#include "InventoryData.generated.h"

UENUM()
enum class EItemType : uint8
{
	IT_Miscellaneous,
	IT_Currency,
	IT_Equipment,
	IT_Consumable
};

UENUM()
enum class EItemRarity : uint8
{
	IR_Common,
	IR_Uncommon,
	IR_Rare,
	IR_Epir,
	IR_Legendary
};

class UInventoryCellWidget;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemDrop, UInventoryCellWidget* /*from*/, UInventoryCellWidget* /*to*/)

USTRUCT(BlueprintType)
struct FInventoryItemInfo : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "General")
	FName Id;

	UPROPERTY(EditAnywhere, Category = "General")
	FText Name;

	//UPROPERTY(EditAnywhere, Category = "General")
	//FText Description;

	UPROPERTY(EditAnywhere, Category = "Type")
	EItemType Type;

	UPROPERTY(EditAnywhere, Category = "Type")
	EItemRarity Rarity;

	UPROPERTY(EditAnywhere, Category = "Visual")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, Category = "Visual")
	TSoftObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere, Category = "Stats")
	int32 Damage;

	UPROPERTY(EditAnywhere, Category = "Stats")
	int32 Armor;

	UPROPERTY(EditAnywhere, Category = "Stats")
	int32 Intelligence;

};

USTRUCT(BlueprintType)
struct FInventorySlotInfo : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Id = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Count;
		
};