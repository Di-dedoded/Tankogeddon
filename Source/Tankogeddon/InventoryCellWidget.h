// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryData.h"
#include "Blueprint/UserWidget.h"
#include "InventoryCellWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class TANKOGEDDON_API UInventoryCellWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage* ItemImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* CountText;

	bool bHasItem;

	UPROPERTY()
	FInventorySlotInfo Item;

public:

	bool HasItem() const { return bHasItem; }

	bool AddItem(const FInventorySlotInfo& InSlot, const FInventoryItemInfo& Info);

	void Clear();

	const FInventorySlotInfo& GetItem() const { return Item; }

	int32 IndexInInventory = INDEX_NONE;

	FOnItemDrop OnItemDrop;

protected:

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	UPROPERTY(EditAnywhere)
	bool bCanDrag = true;
};
