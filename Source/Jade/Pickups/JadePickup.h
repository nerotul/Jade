// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Jade/Pickups/PickupInterface.h"
#include "JadePickup.generated.h"

class USphereComponent;

UCLASS()
class JADE_API AJadePickup : public AActor, public IPickupInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJadePickup();

	virtual void Interact(AActor* Interactor) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* CollisionComponent = nullptr;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
