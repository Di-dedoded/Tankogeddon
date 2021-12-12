// Copyright Epic Games, Inc. All Rights Reserved.


#include "TankogeddonGameModeBase.h"
#include "Scoreable.h"
#include "TankPlayerController.h"
#include "Tankogeddon.h"

void ATankogeddonGameModeBase::NotifyActorWasDestroyedByDamage(AActor* Actor, const FDamageData& DamageData)
{
    if (IScoreable* Scoreable = Cast<IScoreable>(Actor))
    {
        ATankPlayerController* PlayerController = Cast<ATankPlayerController>(GetWorld()->GetFirstPlayerController());
        if (DamageData.Instigator == PlayerController->GetPawn())
        {
            PlayerController->Scores += Scoreable->GetScores();
            UE_LOG(LogTankogeddon, Log, TEXT("New player scores: %d"), PlayerController->Scores);
        }
    }
}
