// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSExtractionZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"

// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
    CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
    CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    CollisionComp->SetBoxExtent(FVector(200.0f));
    RootComponent = CollisionComp;

    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::HandleOverlap);

    Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
    Decal->DecalSize = FVector(200.0f);
    Decal->SetupAttachment(RootComponent);
}

void AFPSExtractionZone::HandleOverlap(UPrimitiveComponent *OverlappedComponent,
                                       AActor *OtherActor,
                                       UPrimitiveComponent *OtherComp,
                                       int32 OtherBodyIndex,
                                       bool bFromSweep,
                                       const FHitResult &SweepResult)
{
    UE_LOG(LogTemp, Log, TEXT("Overlapped with extraction zone"));

    AFPSCharacter *MyCharacter = Cast<AFPSCharacter>(OtherActor);
    if (MyCharacter && MyCharacter->IsCarryingObjective)
    {
        AFPSGameMode *GameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->CompleteMission(MyCharacter);
        }
    }
}
