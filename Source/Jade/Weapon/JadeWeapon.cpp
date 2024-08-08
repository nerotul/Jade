// Fill out your copyright notice in the Description page of Project Settings.


#include "Jade/Weapon/JadeWeapon.h"

// Sets default values
AJadeWeapon::AJadeWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJadeWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJadeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
