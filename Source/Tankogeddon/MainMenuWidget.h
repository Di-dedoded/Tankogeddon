// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MainMenuWidget.generated.h"


/**
 * 
 */
UCLASS()
class TANKOGEDDON_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;

protected:
    UPROPERTY(meta = (BindWidget))
    class UButton* NewGameBtn;
    
    UPROPERTY(meta = (BindWidgetOptional))
    class UButton* OptionsBtn;
    
    UPROPERTY(meta = (BindWidget))
    class UButton* QuitBtn;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
    class UWidgetAnimation* Title;

    UFUNCTION()
    void OnNewGameClicked();

    UFUNCTION()
    void OnQuitClicked();
};
