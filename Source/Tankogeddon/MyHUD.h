// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UENUM()
enum class EWidgetID : uint8
{
    None,
    MainMenu,
    PauseMenu,
    Settings
};

UCLASS()
class TANKOGEDDON_API AMyHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(EditAnywhere)
    TMap<EWidgetID, TSubclassOf<UUserWidget>> WidgetClases;

    UPROPERTY()
    UUserWidget* CurrentWidget;

    EWidgetID CurrentWidgetID;

    EWidgetID PreviousWidgetID;    

public:
    void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    UUserWidget* UseWidget(EWidgetID widgetID, bool RemovePrevious = true, int32 ZOrder = 0);

    UFUNCTION(BlueprintPure)
    UUserWidget* GetCurrentWidget();
    
    UFUNCTION(BlueprintPure)
    EWidgetID GetPreviousWidgetID();

    UFUNCTION(BlueprintCallable)
    void RemoveCurrentWidgetFromViewport();

    UFUNCTION(BlueprintCallable)
    UUserWidget* AddWidgetToViewportByClass(TSubclassOf<UUserWidget> WidgetClass, int32 ZOrder = 0);

};
