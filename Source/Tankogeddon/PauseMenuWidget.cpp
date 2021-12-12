// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

void UPauseMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ResumeGameBtn)
    {
        ResumeGameBtn->OnClicked.AddDynamic(this,
            &UPauseMenuWidget::OnResumeGameClicked);
    }
    if (QuitBtn)
    {
        QuitBtn->OnClicked.AddDynamic(this,
            &UPauseMenuWidget::OnQuitClicked);
    }
}

void UPauseMenuWidget::OnResumeGameClicked()
{
    APlayerController* ATankPlayerController = GetWorld()->GetFirstPlayerController();
    ATankPlayerController->SetInputMode(FInputModeGameOnly());
    ATankPlayerController->bShowMouseCursor = false;
    ATankPlayerController->SetPause(false);
    this->RemoveFromViewport();
}

void UPauseMenuWidget::OnQuitClicked()
{
    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}
