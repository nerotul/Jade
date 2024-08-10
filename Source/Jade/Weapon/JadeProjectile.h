// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JadeProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class AJadeCharacter;

UCLASS()
class JADE_API AJadeProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJadeProjectile();

	USphereComponent* GetCollisionComp() const { return CollisionComponent; }

	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	void SetInstigatorController(AController* InInstigator);

	AController* GetInstigatorController() { return InstigatorController; };

	void SetProjectileDamage(float InDamage);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY()
	float ProjectileDamage = 3.f;

	UPROPERTY()
	AController* InstigatorController = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
