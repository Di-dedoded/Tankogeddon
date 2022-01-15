// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "TankPawn.h"
#include "DrawDebugHelpers.h"
#include "ActorPoolSubsystem.h"

void ATankPlayerController::BeginPlay()
{
    Super::BeginPlay();

    TankPawn = Cast<ATankPawn>(GetPawn());
    bShowMouseCursor = true;

    bEnableClickEvents = true;
}

void ATankPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindKey(EKeys::LeftMouseButton, IE_Released, this, &ATankPlayerController::OnLeftMouseButton);

    InputComponent->BindAxis("MoveForward", this, &ATankPlayerController::MoveForward);
    InputComponent->BindAxis("RotateRight", this, &ATankPlayerController::RotateRight);
    InputComponent->BindAction("Fire", IE_Pressed, this, &ATankPlayerController::Fire);
    InputComponent->BindAction("FireSpecial",IE_Pressed, this, &ATankPlayerController::FireSpecial);
    InputComponent->BindAction("CycleCannon",IE_Pressed, this, &ATankPlayerController::CycleCannon);
    InputComponent->BindAxis("RotateTurretRight");
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!TankPawn)
    {
        return;
    }

    FVector2D MouseScreenPosition;
    GetMousePosition(MouseScreenPosition.X, MouseScreenPosition.Y);
    bool bWasMouseMoved = !LastFrameMousePosition.Equals(MouseScreenPosition);
    LastFrameMousePosition = MouseScreenPosition;

    float TurretRotationAxis = GetInputAxisValue("RotateTurretRight");
    if (FMath::IsNearlyZero(TurretRotationAxis) && (bWasMouseMoved || bIsControllingFromMouse))
    {
        bIsControllingFromMouse = true;
        FVector WorldMousePosition, MouseDirection;
        DeprojectMousePositionToWorld(WorldMousePosition, MouseDirection);

        FVector PawnPos = TankPawn->GetActorLocation();
        WorldMousePosition.Z = PawnPos.Z;
        FVector NewTurretDirection = WorldMousePosition - PawnPos;
        NewTurretDirection.Normalize();

        FVector TurretTarget = PawnPos + NewTurretDirection * 1000.f;
        TankPawn->SetTurretTargetPosition(TurretTarget);
    }
    else
    {
        bIsControllingFromMouse = false;
        TankPawn->SetTurretRotationAxis(TurretRotationAxis);
    }

    DrawDebugLine(GetWorld(), TankPawn->GetActorLocation(), TankPawn->GetActorLocation() + TankPawn->GetTurretForwardVector() * 1000.f, FColor::Green, false, 0.1f, 0.f, 5.f);
}

void ATankPlayerController::OnLeftMouseButton()
{
    OnMouseButtonUp.Broadcast();
}

void ATankPlayerController::MoveForward(float InAxisValue)
{
    if (TankPawn)
    {
        TankPawn->MoveForward(InAxisValue);
    }
}

void ATankPlayerController::RotateRight(float InAxisValue)
{
    if (TankPawn)
    {
        TankPawn->RotateRight(InAxisValue);
    }
}

void ATankPlayerController::Fire()
{
    if (TankPawn)
    {
        TankPawn->Fire();
    }
}

void ATankPlayerController::FireSpecial()
{
    if (TankPawn)
    {
        TankPawn->FireSpecial();
    }
}

void ATankPlayerController::CycleCannon()
{
    if (TankPawn)
    {
        TankPawn->CycleCannon();
    }
}

void ATankPlayerController::DumpActorPoolSubsystemStats()
{
    GetWorld()->GetSubsystem<UActorPoolSubsystem>()->DumpPoolStats();
}
