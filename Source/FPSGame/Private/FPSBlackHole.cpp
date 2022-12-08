// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSBlackHole.h"
#include "Components/SphereComponent.h"

// Sets default values
AFPSBlackHole::AFPSBlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RootComponent = MeshComp;

    InnerSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("InnerSphereComponent"));
    InnerSphereComponent->SetSphereRadius(100);
    InnerSphereComponent->SetupAttachment(MeshComp);
    InnerSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHole::OverlapInnerSphere);


    OuterSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("OuterSphereComp"));
    OuterSphereComponent->SetSphereRadius(3000);
    OuterSphereComponent->SetupAttachment(MeshComp);

}

void AFPSBlackHole::OverlapInnerSphere(UPrimitiveComponent *OverlappedComponent,
                                       AActor *OtherActor,
                                       UPrimitiveComponent *OtherComp,
                                       int32 OtherBodyIndex,
                                       bool bFromSweep,
                                       const FHitResult &SweepResult)
{
    if (OtherActor)
    {
        OtherActor->Destroy();
    }
}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    TArray<UPrimitiveComponent*> OverlappingComponents;
    OuterSphereComponent->GetOverlappingComponents(OverlappingComponents);

    for (int32 i = 0; i < OverlappingComponents.Num(); ++i)
    {
        UPrimitiveComponent *Component = OverlappingComponents[i];
        if (Component && Component->IsSimulatingPhysics())
        {
            const float SphereRadius = OuterSphereComponent->GetScaledSphereRadius();
            const float ForceStrength = -2000;

            Component->AddRadialForce(GetActorLocation(), SphereRadius, ForceStrength, ERadialImpulseFalloff::RIF_Constant, true);
        }
    }
    // for (auto &it : OverlappingComponents)
    // {
    //     it->AddRadialForce(CollisionComp->GetComponentLocation(),
    //                        CollisionComp->GetScaledSphereRadius() * 100.0f,
    //                        2.0f,
    //                        ERadialImpulseFalloff::RIF_Linear,
    //                        false);
    // }
}

