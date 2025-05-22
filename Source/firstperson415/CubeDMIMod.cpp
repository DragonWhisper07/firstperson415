#include "CubeDMIMod.h"
#include "firstperson415Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
ACubeDMIMod::ACubeDMIMod()
{
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	RootComponent = boxComp;

	cubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube Mesh"));
	cubeMesh->SetupAttachment(RootComponent);
}

void ACubeDMIMod::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ACubeDMIMod::OnOverlapBegin);

	if (baseMat)
	{
		dmiMat = UMaterialInstanceDynamic::Create(baseMat, this);

		if (cubeMesh && dmiMat)
		{
			cubeMesh->SetMaterial(0, dmiMat);
		}
	}
}

void ACubeDMIMod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACubeDMIMod::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Afirstperson415Character* overlappedActor = Cast<Afirstperson415Character>(OtherActor);

	if (overlappedActor && dmiMat)
	{
		FLinearColor randColor = FLinearColor::MakeRandomColor();
		float randDarkness = randColor.R;

		dmiMat->SetVectorParameterValue("Color", randColor);
		dmiMat->SetScalarParameterValue("Darkness", randDarkness);

		if (colorP)
		{
			UNiagaraComponent* particleComp = UNiagaraFunctionLibrary::SpawnSystemAttached(colorP, OtherComp, NAME_None, FVector(-20.f, 0.f, 0.f), FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);

			if (particleComp)
			{
				particleComp->SetVariableLinearColor(FName("RandColor"), randColor);
			}
		}
	}
}
