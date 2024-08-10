// Fill out your copyright notice in the Description page of Project Settings.


#include "JadePickup.h"
#include "Components/SphereComponent.h"

// Sets default values
AJadePickup::AJadePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(30.0f);
	CollisionComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComponent->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(CollisionComponent);
}

void AJadePickup::Interact(AActor* Interactor)
{

}

// Called when the game starts or when spawned
void AJadePickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJadePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

