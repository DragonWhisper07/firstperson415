#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "NiagaraSystem.h"
#include "CubeDMIMod.generated.h"

UCLASS()
class FIRSTPERSON415_API ACubeDMIMod : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACubeDMIMod();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* cubeMesh;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* baseMat;

	UPROPERTY()
	UMaterialInstanceDynamic* dmiMat;

	// Add this Niagara system property to assign in editor
	UPROPERTY(EditAnywhere, Category = "Effects")
	UNiagaraSystem* colorP;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
