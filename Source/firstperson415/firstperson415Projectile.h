// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "firstperson415Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;
class UStaticMeshComponent;
class UMaterialInterface;
class UMaterialInstanceDynamic;
class UMaterial;

UCLASS(config = Game)
class FIRSTPERSON415_API Afirstperson415Projectile : public AActor
{
	GENERATED_BODY()

public:
	Afirstperson415Projectile();

protected:
	virtual void BeginPlay() override;

	/** Called when projectile hits something */
	UFUNCTION()
	void OnHit(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);

public:
	/** Returns CollisionComp subobject */
	USphereComponent* GetCollisionComp() const { return CollisionComp; }

	/** Returns ProjectileMovement subobject */
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

protected:
	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ballMesh;

	UPROPERTY(EditAnywhere)
	UMaterial* baseMat;

	UPROPERTY()
	FLinearColor randColor;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* projMat;

	UPROPERTY()
	UMaterialInstanceDynamic* dmiMat;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* colorP;
};