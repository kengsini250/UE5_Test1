// Fill out your copyright notice in the Description page of Project Settings.


#include "NewLevel.h"

#include "MainCharacter.h"
#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ANewLevel::ANewLevel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	TransitionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TransitionVolume"));
	RootComponent = TransitionVolume;
	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(GetRootComponent());
	//默认初始地图名
	MapName = "";
}

// Called when the game starts or when spawned
void ANewLevel::BeginPlay()
{
	Super::BeginPlay();
	TransitionVolume->OnComponentBeginOverlap.AddDynamic(this,&ANewLevel::OnOverlapBegin);
}

// Called every frame
void ANewLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANewLevel::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		AMainCharacter*mainC = Cast<AMainCharacter>(OtherActor);
		if(mainC)
		{
			mainC->switchLevel(MapName);
		}
	}
}

