// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "PauseMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

protected:

    UPROPERTY(meta = (BindWidget))
    class UButton* ResumeGameBtn;

    UPROPERTY(meta = (BindWidgetOptional))
    class UButton* OptionsBtn;

    UPROPERTY(meta = (BindWidget))
    class UButton* QuitBtn;

    UFUNCTION()
    void OnResumeGameClicked();

    UFUNCTION()
    void OnQuitClicked();
};
