// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Tankogeddon.h"
#include "Damageable.h"
#include "ActorPoolSubsystem.h"


// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickInterval = 0.005f;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->OnComponentHit.AddDynamic(this, &AProjectile::OnMeshHit);
	Mesh->SetVisibility(true);
	RootComponent = Mesh;
}


void AProjectile::Start()
{
	PrimaryActorTick.SetTickFunctionEnable(true);
	StartPosition = GetActorLocation();
	Mesh->SetVisibility(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AProjectile::Stop()
{
	PrimaryActorTick.SetTickFunctionEnable(false);
	Mesh->SetVisibility(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UActorPoolSubsystem* Pool = GetWorld()->GetSubsystem<UActorPoolSubsystem>();
	if (Pool->IsActorInPool(this))
	{
		Pool->ReturnActor(this);
	}
	else
	{
		Destroy();
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * DeltaTime;
	SetActorLocation(NextPosition, true);

	if (FVector::Dist(GetActorLocation(), StartPosition) > FireRange)
	{
		Stop();
	}
}

void AProjectile::OnMeshHit(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	UE_LOG(LogTankogeddon, Warning, TEXT("Projectile %s collided with %s. "), *GetName(), *OtherActor->GetName());

	//if (OtherActor == GetInstigator())
	if (bEnableRadialDamage)
	{
		FVector StartPos = GetActorLocation();
		FVector EndPos = StartPos + FVector(0.1f);

		FCollisionShape Shape = FCollisionShape::MakeSphere(ExplosionRange);
		FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
		Params.AddIgnoredActor(this);
		Params.bTraceComplex = true;
		Params.TraceTag = "Explode Trace";
		TArray<FHitResult> AttackHit;

		FQuat Rotation = FQuat::Identity;

		GetWorld()->DebugDrawTraceTag = "Explode Trace";

		bool bSweepResult = GetWorld()->SweepMultiByChannel
		(
			AttackHit,
			StartPos,
			EndPos,
			Rotation,
			ECollisionChannel::ECC_Visibility,
			Shape,
			Params
		);

		if (bSweepResult)
		{
			for (FHitResult SweepHitResult : AttackHit)
			{
				AActor* HitActor = SweepHitResult.GetActor();
				if (!HitActor)
					continue;

				FVector ForceVector = HitActor->GetActorLocation() - GetActorLocation();
				ApplyDamage(HitActor, Cast<UPrimitiveComponent>(HitActor->GetRootComponent()), SweepHitResult, ForceVector * ExplosionImpulse);
			}
		}
	}
	else
	{
		ApplyDamage(OtherActor, OtherComp, HitResult, Mass * MoveSpeed * GetActorForwardVector());
	}

	Stop();
}

void AProjectile::ApplyDamage(AActor* Actor, UPrimitiveComponent* Component, const FHitResult& Hit, const FVector& Impulse)
{
	if (Actor == GetInstigator())
	{
		//Destroy();
		return;
	}

	if (Component && Component->IsSimulatingPhysics())
	{
		//FVector Impulse = Mass * MoveSpeed * GetActorForwardVector();
		//OtherComp->AddImpulseAtLocation(Impulse, HitResult.ImpactPoint);
		Component->AddImpulseAtLocation(Impulse, Hit.ImpactPoint);
	}

	if (Actor && Component && Component->GetCollisionObjectType() == ECC_Destructible)
	{
		Actor->Destroy();
	}
	else if (IDamageable* Damageable = Cast<IDamageable>(Actor))
	{
		FDamageData DamageData;
		DamageData.DamageValue = Damage;
		DamageData.Instigator = GetInstigator();
		DamageData.DamageMaker = this;
		Damageable->TakeDamage(DamageData);
	}

	//Stop();
}
