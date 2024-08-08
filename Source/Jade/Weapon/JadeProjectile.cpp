// Fill out your copyright notice in the Description page of Project Settings.


#include "Jade/Weapon/JadeProjectile.h"

// Sets default values
AJadeProjectile::AJadeProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJadeProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJadeProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

