// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

// Sets default values for this component's properties

void UInventoryComponent::SetItem(int32 SlotIndex, const FInventorySlotInfo& Item)
{
	ClearItem(SlotIndex);
	Items.Add(SlotIndex, Item);

}

void UInventoryComponent::ClearItem(int32 SlotIndex)
{
	Items.Remove(SlotIndex);
}
