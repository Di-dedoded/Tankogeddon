// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagerComponent.h"
#include "InventoryComponent.h"
#include "InventoryWidget.h"
#include "InventoryCellWidget.h"

UInventoryManagerComponent::UInventoryManagerComponent()
{
	
}

void UInventoryManagerComponent::Init(UInventoryComponent* InInventoryComponent)
{
	LocalInventoryComponent = InInventoryComponent;

	if (LocalInventoryComponent && ItemsData && InventoryWidgetClass)
	{
		InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
		InventoryWidget->AddToViewport();

		InventoryWidget->Init(FMath::Max(MinInventorySize, LocalInventoryComponent->GetItemNum()));
		InventoryWidget->OnItemDrop.AddUObject(this, &ThisClass::OnItemDropFunc);

		for (const auto& Item : LocalInventoryComponent->GetItems())
		{
			auto* Data = GetItemData(Item.Value.Id);
			if (Data)
			{
				InventoryWidget->AddItem(Item.Value, *Data, Item.Key);
			}
		}

	}
}

const FInventoryItemInfo* UInventoryManagerComponent::GetItemData(const FName& InID) const
{
	return ItemsData ? ItemsData->FindRow<FInventoryItemInfo>(InID, "") : nullptr;
}

void UInventoryManagerComponent::OnItemDropFunc(UInventoryCellWidget* From, UInventoryCellWidget* To)
{
	FInventorySlotInfo FromItem =  From->GetItem();
	FInventorySlotInfo ToItem = To->GetItem();

	From->Clear();
	To->Clear();

	To->AddItem(FromItem, *GetItemData(FromItem.Id));

	if (!ToItem.Id.IsNone())
	{
		From->AddItem(ToItem, *GetItemData(ToItem.Id));
	}
}
