// Fill out your copyright notice in the Description page of Project Settings.


#include "RocketProjectile.h"
#include "Components/PrimitiveComponent.h"

ARocketProjectile::ARocketProjectile()
{
    Mesh->BodyInstance.SetInstanceSimulatePhysics(true);
    Mesh->BodyInstance.bLockZTranslation = true;
    Mesh->SetEnableGravity(false);
}

void ARocketProjectile::Tick(float DeltaSeconds)
{
    Mesh->AddForce(GetActorForwardVector() * EngineThrust);

    if (FVector::Dist(GetActorLocation(), StartPosition) > FireRange)
    {
        Stop();
    }
}
