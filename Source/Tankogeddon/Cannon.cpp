// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Tankogeddon.h"
#include "Projectile.h"
#include "DrawDebugHelpers.h"

// Sets default values
ACannon::ACannon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceeneCpm;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);
}

void ACannon::Fire()
{
	if (!IsReadyToFire())
	{
		return;
	}
	bIsReadyToFire = false;
	--NumAmmo;
	ShotsLeft = NumShotsInSeries;
	Shot();

	UE_LOG(LogTankogeddon, Log, TEXT("Fire! Ammo left: %d"), NumAmmo);
}

void ACannon::FireSpecial()
{
	if (!HasSpecialFire() || !IsReadyToFire())
	{
		return;
	}

	bIsReadyToFire = false;
	--NumAmmo;

	if (Type == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, TEXT("Fire special - projectile"));

		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
		if (Projectile)
		{
			Projectile->Start();
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, TEXT("Fire special - trace"));
	}

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1.f / FireRate, false);
	UE_LOG(LogTankogeddon, Log, TEXT("FireSpecial! Ammo left: %d"), NumAmmo);
}

bool ACannon::IsReadyToFire()
{
	return bIsReadyToFire && NumAmmo > 0 && ShotsLeft == 0;
}

bool ACannon::HasSpecialFire() const
{
	return bHasSpecialFire;
}

void ACannon::SetVisibility(bool bIsVisible)
{
	Mesh->SetHiddenInGame(!bIsVisible);
}

void ACannon::AddAmmo(int32 InNumAmmo)
{
	NumAmmo = FMath::Clamp(NumAmmo + InNumAmmo, 0, MaxAmmo);
	UE_LOG(LogTankogeddon, Log, TEXT("AddAmmo(%d)! NumAmmo: %d"), InNumAmmo, NumAmmo);
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();

	bIsReadyToFire = true;
	NumAmmo = 10;
	ShotsLeft = 0;
}

void ACannon::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(SeriesTimerHandle);
}

void ACannon::Reload()
{
	bIsReadyToFire = true;
}

void ACannon::Shot()
{
	check(ShotsLeft > 0);
	if (Type == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1, FColor::Green, TEXT("Fire - projectile"));

		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
		if (Projectile)
		{
			Projectile->Start();
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1, FColor::Green, TEXT("Fire - trace"));

		FHitResult HitResult;
		FVector TraceStart = ProjectileSpawnPoint->GetComponentLocation();
		FVector TraceEnd = ProjectileSpawnPoint->GetComponentLocation() + ProjectileSpawnPoint->GetForwardVector() * FireRange;
		FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
		TraceParams.bReturnPhysicalMaterial = false;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, TraceParams))
		{
			DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, FColor::Red, false, 0.5f, 0, 5.f);
			if (HitResult.Actor.IsValid() && HitResult.Component.IsValid(), HitResult.Component->GetCollisionObjectType() == ECC_Destructible)
			{
				HitResult.Actor->Destroy();
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.5f, 0, 5.f);
		}
	}

	if (--ShotsLeft > 0)
	{
		const float NextShotTime = SeriesLength / (NumShotsInSeries - 1);
		GetWorld()->GetTimerManager().SetTimer(SeriesTimerHandle, this, &ACannon::Shot, NextShotTime, false);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1.f / FireRate, false);
	}
}