// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void AMyHUD::BeginPlay()
{
    Super::BeginPlay();

    if (GetWorld())
    {
        APlayerController* PC = GetWorld()->GetFirstPlayerController();
        if (PC)
        {
            UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PC, nullptr, EMouseLockMode::DoNotLock, false);
            PC->bShowMouseCursor = true;
        }
    }
}

UUserWidget* AMyHUD::GetCurrentWidget()
{
    return CurrentWidget;
}

EWidgetID AMyHUD::GetPreviousWidgetID()
{
    return PreviousWidgetID;
}

void AMyHUD::RemoveCurrentWidgetFromViewport()
{
    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;

        CurrentWidgetID = EWidgetID::None;
    }
}

UUserWidget* AMyHUD::AddWidgetToViewportByClass(TSubclassOf<UUserWidget> WidgetClass, int32 ZOrder /*= 0*/)
{
    CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);

    if (CurrentWidget)
    {
        CurrentWidget->AddToViewport(ZOrder);
    }

    return CurrentWidget;
}


UUserWidget* AMyHUD::UseWidget(EWidgetID widgetID, bool RemovePrevious, int32 ZOrder)
{
    PreviousWidgetID = CurrentWidgetID;
    TSubclassOf<UUserWidget> WidgetClassToUse = WidgetClases.FindRef(widgetID);
    if (CurrentWidgetID == widgetID)
    {
        return AddWidgetToViewportByClass(WidgetClassToUse, ZOrder);
    }

    if (RemovePrevious)
    {
        RemoveCurrentWidgetFromViewport();
    }

    if (WidgetClassToUse.Get())
    {
        CurrentWidgetID = widgetID;
        return AddWidgetToViewportByClass(WidgetClassToUse, ZOrder);
    };

    return nullptr;
}

