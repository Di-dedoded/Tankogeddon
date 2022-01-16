// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "Components/UniformGridPanel.h"
#include "InventoryCellWidget.h"



void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInventoryWidget::Init(int32 ItemsCount)
{
	if (CellsPanel)
	{
		CellsPanel->ClearChildren();
		for (int32 i = 0; i < ItemsCount; i++)
		{
			if (auto* Cell = CreateCell())
			{
				Cell->IndexInInventory = i;
				CellsPanel->AddChildToUniformGrid(Cell, i / ItemsInRow, i % ItemsInRow);
			}
		}

	}
}

bool UInventoryWidget::AddItem(const FInventorySlotInfo& InSlot, const FInventoryItemInfo& Info, int32 SlotIndex)
{
	if (Info.Type == EItemType::IT_Currency)
	{
		if (GoldCell)
		{
			return GoldCell->AddItem(InSlot, Info);
		}
	}

	if (CellsPanel)
	{
		UInventoryCellWidget* Found = nullptr;
		UInventoryCellWidget** FoundPtr = CellWidgets.FindByPredicate([SlotIndex](UInventoryCellWidget* Cell)
			{
				return Cell && Cell->IndexInInventory == SlotIndex;
			});
		if (FoundPtr)
		{
			Found = *FoundPtr;
		}
		else
		{
			for (auto* Cell : CellWidgets)
			{
				if (!Cell->HasItem())
				{
					Found = Cell;
					break;
				}
			}
		}

		if (Found)
		{
			return Found->AddItem(InSlot, Info);
		}


	}

	return false;
}

UInventoryCellWidget* UInventoryWidget::CreateCell()
{
	if (CellWidgetClass)
	{
		auto* Cell = CreateWidget<UInventoryCellWidget>(this, CellWidgetClass);
		CellWidgets.Add(Cell);
		Cell->OnItemDrop.AddUObject(this, &ThisClass::OnItemDropFunc);
		return Cell;
	}

	return nullptr;
}

void UInventoryWidget::OnItemDropFunc(UInventoryCellWidget* From, UInventoryCellWidget* To)
{
	OnItemDrop.Broadcast(From, To);
}
