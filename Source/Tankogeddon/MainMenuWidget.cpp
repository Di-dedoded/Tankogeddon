// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (NewGameBtn)
    {
        NewGameBtn->OnClicked.AddDynamic(this,
            &UMainMenuWidget::OnNewGameClicked);
    }
    if (QuitBtn)
    {
        QuitBtn->OnClicked.AddDynamic(this,
            &UMainMenuWidget::OnQuitClicked);
    }
}

void UMainMenuWidget::OnNewGameClicked()
{
    APlayerController* ATankPlayerController = GetWorld()->GetFirstPlayerController();
    ATankPlayerController->SetInputMode(FInputModeGameOnly());
    UGameplayStatics::OpenLevel(this, "M_Level_1", true);
}

void UMainMenuWidget::OnQuitClicked()
{
    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}
