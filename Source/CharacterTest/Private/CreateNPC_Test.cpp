// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateNPC_Test.h"

#include "NPC_Test.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"

// Sets default values
ACreateNPC_Test::ACreateNPC_Test()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Base = CreateDefaultSubobject<UBoxComponent>(TEXT("Base"));
}

// Called when the game starts or when spawned
void ACreateNPC_Test::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACreateNPC_Test::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ACreateNPC_Test::GetRandomPoint()
{
	FVector BoxSize = Base->GetScaledBoxExtent();
	FVector Origin = Base->GetComponentLocation();
	FVector Point = UKismetMathLibrary::RandomPointInBoundingBox(Origin, BoxSize);

	return Point;
}

void ACreateNPC_Test::CreateNPC_Implementation(UClass* NewNPC, const FVector& Location)
{
		if(NewNPC)
		{
			UWorld* World = GetWorld();
			FActorSpawnParameters p;
			if(World)
			{
				ANPC_Test* NewNPC_temp =  World->SpawnActor<ANPC_Test>(NewNPC,Location,FRotator(0),p);
			}
		}
}
