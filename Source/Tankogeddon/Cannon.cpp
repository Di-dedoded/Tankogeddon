// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Tankogeddon.h"
#include "Projectile.h"
#include "DrawDebugHelpers.h"
#include "ActorPoolSubsystem.h"
#include "Damageable.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Camera/CameraShakeBase.h"
#include "GameFramework/ForceFeedbackEffect.h"
#include "PhysicsProjectile.h"
#include "PhysicsMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

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

    ShootEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Shoot Effect"));
    ShootEffect->SetupAttachment(ProjectileSpawnPoint);

    AudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Effect"));
    AudioEffect->SetupAttachment(ProjectileSpawnPoint);
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

float ACannon::GetAmmoState() const
{
    return static_cast<float>(NumAmmo) / static_cast<float>(MaxAmmo);
}

int32 ACannon::GetAmmo() const
{
    return NumAmmo;
}

bool ACannon::IsBallistic() const
{
    return Type == ECannonType::FireProjectile && ProjectileClass && ProjectileClass->IsChildOf<APhysicsProjectile>();
}

FVector ACannon::GetCurrentBallisticTarget(float FloorAbsoluteHeight) const
{
    if (!IsBallistic())
    {
        // Can't calculate
        return GetActorLocation();
    }

    APhysicsProjectile* DefaultProjectile = ProjectileClass->GetDefaultObject<APhysicsProjectile>();
    check(DefaultProjectile);

    float Angle = FMath::DegreesToRadians(ProjectileSpawnPoint->GetForwardVector().Rotation().Pitch);
    float Speed = DefaultProjectile->MoveSpeed;
    float Gravity = DefaultProjectile->MovementComponent->Gravity;
    if (FMath::IsNearlyZero(Gravity))
    {
        // Can't calculate
        return GetActorLocation();
    }

    float Z = ProjectileSpawnPoint->GetComponentLocation().Z - FloorAbsoluteHeight;

    // From https://habr.com/ru/post/538952/ (y is calculated differently)
    float SqrtComp = FMath::Sqrt(FMath::Square(Speed * FMath::Sin(Angle)) + 2 * Gravity * Z);
    float Range = Speed * FMath::Cos(Angle) * (Speed * FMath::Sin(Angle) + SqrtComp) / Gravity;
    FVector FlatDirection = ProjectileSpawnPoint->GetForwardVector();
    FlatDirection.Z = 0.f;
    FlatDirection.Normalize();
    FVector Result = ProjectileSpawnPoint->GetComponentLocation() + FlatDirection * Range;
    Result.Z = FloorAbsoluteHeight;
    return Result;
}

bool ACannon::SetDesiredBallisticTarget(const FVector& InTarget)
{
    if (!IsBallistic())
    {
        // Nothing to do with non ballistic cannons
        return false;
    }

    APhysicsProjectile* DefaultProjectile = ProjectileClass->GetDefaultObject<APhysicsProjectile>();
    check(DefaultProjectile);

    float Speed = DefaultProjectile->MoveSpeed;
    float Gravity = DefaultProjectile->MovementComponent->Gravity;
    if (FMath::IsNearlyZero(Gravity))
    {
        // Can't do anything with zero gravity
        return false;
    }
    float Z = ProjectileSpawnPoint->GetComponentLocation().Z - InTarget.Z;
    float X = FVector::Dist2D(ProjectileSpawnPoint->GetComponentLocation(), InTarget);
    float Angle = 90.f; // If X is 0 then just look at the sky :)
    if (!FMath::IsNearlyZero(X))
    {
        float FirstSqrtComp = FMath::Pow(Speed, 4);
        float SecondSqrtComp = Gravity * (Gravity * FMath::Square(X) - 2 * (FMath::Square(Speed) * Z));
        float SqrtComp = 0.f;
        if (FirstSqrtComp > SecondSqrtComp)
        {
            SqrtComp = FMath::Sqrt(FirstSqrtComp - SecondSqrtComp);
        }
        Angle = FMath::Atan((FMath::Square(Speed) + SqrtComp) / (Gravity * X));
        Angle = FMath::RadiansToDegrees(Angle);
    }
   
    FRotator DesiredRotation = GetActorRotation();
    DesiredRotation.Pitch = Angle;
    SetActorRotation(DesiredRotation);
    return true;
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	
    bIsReadyToFire = true;
    NumAmmo = MaxAmmo;
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
    ShootEffect->ActivateSystem();
    AudioEffect->Play();

    if (GetOwner() == GetWorld()->GetFirstPlayerController()->GetPawn())
    {
        if (ShootForceEffect)
        {
            FForceFeedbackParameters Params;
            Params.bLooping = false;
            Params.Tag = TEXT("ShootFFParams");
            GetWorld()->GetFirstPlayerController()->ClientPlayForceFeedback(ShootForceEffect);
        }

        if (ShootShake)
        {
            GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(ShootShake);
        }
    }

    if (Type == ECannonType::FireProjectile)
    {
        GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1, FColor::Green, TEXT("Fire - projectile"));

        UActorPoolSubsystem* Pool = GetWorld()->GetSubsystem<UActorPoolSubsystem>();
        FTransform SpawnTransform(ProjectileSpawnPoint->GetComponentRotation(), ProjectileSpawnPoint->GetComponentLocation(), FVector::OneVector);
        AProjectile* Projectile = Cast<AProjectile>(Pool->RetreiveActor(ProjectileClass, SpawnTransform));
        if (Projectile)
        {
            Projectile->SetInstigator(GetInstigator());
            Projectile->Start();
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1, FColor::Green, TEXT("Fire - trace"));

        FHitResult HitResult;
        FVector TraceStart = ProjectileSpawnPoint->GetComponentLocation();
        FVector TraceEnd = ProjectileSpawnPoint->GetComponentLocation() + ProjectileSpawnPoint->GetForwardVector() * FireRange;
        FCollisionQueryParams TraceParams = FCollisionQueryParams (FName(TEXT("FireTrace")), true, this);
        TraceParams.bReturnPhysicalMaterial = false;
        if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, TraceParams))
        {
            DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, FColor::Red, false, 0.5f, 0, 10.f);
            if (HitResult.Actor.IsValid() && HitResult.Component.IsValid(), HitResult.Component->GetCollisionObjectType() == ECC_Destructible)
            {
                HitResult.Actor->Destroy();
            }
            else if (IDamageable* Damageable = Cast<IDamageable>(HitResult.Actor))
            {
                AActor* MyInstigator = GetInstigator();
                if (HitResult.Actor != MyInstigator)
                {
                    FDamageData DamageData;
                    DamageData.DamageValue = FireDamage;
                    DamageData.DamageMaker = this;
                    DamageData.Instigator = MyInstigator;
                    Damageable->TakeDamage(DamageData);
                }
            }
        }
        else
        {
            DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.5f, 0, 10.f);
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
