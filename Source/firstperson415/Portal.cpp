// Fill out your copyright notice in the Description page of Project Settings.

#include "Portal.h"
#include "Components/BoxComponent.h"
#include "firstperson415Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APortal::APortal()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Capture"));
	rootArrow = CreateDefaultSubobject<UArrowComponent>("Root Arrow");

	RootComponent = BoxComp;
	Mesh->SetupAttachment(BoxComp);
	SceneCapture->SetupAttachment(Mesh);
	rootArrow->SetupAttachment(RootComponent);

	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnOverlapBegin);

	Mesh->SetHiddenInSceneCapture(true);
	Mesh->CastShadow;
	//Mesh->bCastStaticShadow(false);
	//Mesh->bCastDynamicShadow(false);
	
	if (Mat)
	{
		Mesh->SetMaterial(0, Mat);
	}
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdatePortals();
}

void APortal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Afirstperson415Character* PlayerChar = Cast<Afirstperson415Character>(OtherActor);

	if (PlayerChar)
	{
		if (OtherPortal)
		{
			if (!PlayerChar->isTeleporting)
			{
				PlayerChar->isTeleporting = true;
				FVector Loc = OtherPortal->rootArrow->GetComponentLocation();
				PlayerChar->SetActorLocation(Loc);

				FTimerHandle TimerHandle;
				FTimerDelegate TimerDelegate;
				TimerDelegate.BindUFunction(this, "SetBool", PlayerChar);
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.0f, false);
			}
		}
	}
}

void APortal::SetBool(Afirstperson415Character* PlayerChar)
{
	if (PlayerChar)
	{
		PlayerChar->isTeleporting = false;
	}
}

void APortal::UpdatePortals()
{
	if (!OtherPortal) return;

	FVector Offset = GetActorLocation() - OtherPortal->GetActorLocation();
	APlayerCameraManager* CamManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	if (!CamManager) return;

	FVector CamLocation = CamManager->GetTransformComponent()->GetComponentLocation();
	FRotator CamRotation = CamManager->GetTransformComponent()->GetComponentRotation();
	FVector AdjustedLocation = CamLocation + Offset;

	SceneCapture->SetWorldLocationAndRotation(AdjustedLocation, CamRotation);
}
