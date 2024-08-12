// Fill out your copyright notice in the Description page of Project Settings.


#include "Jade/Weapon/JadeProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Jade/Character/JadeCharacter.h"

// Sets default values
AJadeProjectile::AJadeProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComponent->OnComponentHit.AddDynamic(this, &AJadeProjectile::OnHit);		// set up a notification for when this component hits something blocking

	CollisionComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComponent->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComponent;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	InitialLifeSpan = 3.0f;
}

void AJadeProjectile::SetInstigatorController(AController* InInstigator)
{
	InstigatorController = InInstigator;
}

void AJadeProjectile::SetProjectileDamage(float InDamage)
{
	ProjectileDamage = InDamage;
}

// Called when the game starts or when spawned
void AJadeProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AJadeProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && OtherComp)
	{
		if (HasAuthority() && IsValid(OtherActor))
		{
			if (OtherActor->GetInstigatorController() != InstigatorController)
			{
				UGameplayStatics::ApplyDamage(OtherActor, ProjectileDamage, InstigatorController, this, NULL);
			}
		}

		Destroy();
	}
}

// Called every frame
void AJadeProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

