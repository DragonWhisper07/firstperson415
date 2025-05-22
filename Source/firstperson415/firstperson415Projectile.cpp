#include "firstperson415Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

Afirstperson415Projectile::Afirstperson415Projectile()
{
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComp->InitSphereRadius(5.0f);
    CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
    CollisionComp->OnComponentHit.AddDynamic(this, &Afirstperson415Projectile::OnHit);
    CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
    CollisionComp->CanCharacterStepUpOn = ECB_No;

    ballMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
    ballMesh->SetupAttachment(CollisionComp);

    RootComponent = CollisionComp;

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    ProjectileMovement->UpdatedComponent = CollisionComp;
    ProjectileMovement->InitialSpeed = 3000.f;
    ProjectileMovement->MaxSpeed = 3000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = true;

    InitialLifeSpan = 3.0f;
}

void Afirstperson415Projectile::BeginPlay()
{
    Super::BeginPlay();

    randColor = FLinearColor(
        UKismetMathLibrary::RandomFloatInRange(0.f, 1.f),
        UKismetMathLibrary::RandomFloatInRange(0.f, 1.f),
        UKismetMathLibrary::RandomFloatInRange(0.f, 1.f),
        1.f);

    dmiMat = UMaterialInstanceDynamic::Create(projMat, this);
    if (dmiMat)
    {
        ballMesh->SetMaterial(0, dmiMat);
        dmiMat->SetVectorParameterValue("ProjColor", randColor);
    }
}

void Afirstperson415Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
    {
        OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
        Destroy();
    }

    if (OtherActor != nullptr)
    {
        if (colorP)
        {
            UNiagaraComponent* particleComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
                colorP,
                HitComp,
                NAME_None,
                FVector(-20.f, 0.f, 0.f),
                FRotator::ZeroRotator,
                EAttachLocation::KeepRelativeOffset,
                true
            );

            if (particleComp)
            {
                particleComp->SetVariableLinearColor(FName("RandomColor"), randColor);
            }

            ballMesh->DestroyComponent();
            CollisionComp->BodyInstance.SetCollisionProfileName("NoCollision");
        }
    }

    // Spawn decal at hit location
    float FrameNum = UKismetMathLibrary::RandomFloatInRange(0.f, 3.f);
    FLinearColor DecalColor(
        UKismetMathLibrary::RandomFloatInRange(0.f, 1.f),
        UKismetMathLibrary::RandomFloatInRange(0.f, 1.f),
        UKismetMathLibrary::RandomFloatInRange(0.f, 1.f),
        1.f);

    UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(
        GetWorld(),
        baseMat,
        FVector(UKismetMathLibrary::RandomFloatInRange(20.f, 40.f)),
        Hit.Location,
        Hit.Normal.Rotation(),
        0.f);

    if (Decal)
    {
        UMaterialInstanceDynamic* MatInstance = Decal->CreateDynamicMaterialInstance();
        if (MatInstance)
        {
            MatInstance->SetVectorParameterValue("Color", DecalColor);
            MatInstance->SetScalarParameterValue("Frame", FrameNum);
        }
    }
}