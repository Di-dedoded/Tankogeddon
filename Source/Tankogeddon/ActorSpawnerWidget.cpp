// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorSpawnerWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include <Tankogeddon/TankPlayerController.h>


void UActorSpawnerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ATankPlayerController* PC =	Cast<ATankPlayerController>(PlayerController);
	PC->OnMouseButtonUp.AddUObject(this, &UActorSpawnerWidget::OnMouseButtonUp);
}

void UActorSpawnerWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (DraggedText)
	{
		DraggedText->SetText(FText::FromString(DraggedName));
	}
}

void UActorSpawnerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

    if (SpawnedActor && PlayerController)
    {
        FVector WorldMouseLocation;
        FVector WorldMouseDirection;
        PlayerController->DeprojectMousePositionToWorld(WorldMouseLocation, WorldMouseDirection);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(SpawnedActor);
		Params.AddIgnoredActor(PlayerController->GetPawn());

        FHitResult HitResult;
        float TraseDistance = 1000000.f;
        GetWorld()->LineTraceSingleByChannel(HitResult, WorldMouseLocation, WorldMouseLocation + WorldMouseDirection * TraseDistance, ECollisionChannel::ECC_WorldStatic, Params);

        if (HitResult.Actor.Get())
        {
            SpawnedActor->SetActorLocation(HitResult.Location);
        }
    }

}

FReply UActorSpawnerWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		SpawnedActor = GetWorld()->SpawnActor<AActor>(SpawnerClass);
	}
	return FReply::Handled();
}

void UActorSpawnerWidget::OnMouseButtonUp()
{
	if (SpawnedActor)
	{
		SpawnedActor = nullptr;
	}
}
