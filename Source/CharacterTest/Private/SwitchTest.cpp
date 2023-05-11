// Fill out your copyright notice in the Description page of Project Settings.


#include "SwitchTest.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASwitchTest::ASwitchTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;

	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Box->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	Box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	Box->SetBoxExtent(FVector(60, 60, 30));

	Switch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Switch"));
	Switch->SetupAttachment(GetRootComponent());

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ASwitchTest::BeginPlay()
{
	Super::BeginPlay();

	//Switch->OnComponentBeginOverlap ?
	Box->OnComponentBeginOverlap.AddDynamic(this, &ASwitchTest::OnOverlapBegin);
	Box->OnComponentEndOverlap.AddDynamic(this, &ASwitchTest::OnOverlapEnd);

	InitDoorLoaction = Door->GetComponentLocation();
	InitSwitchLocation = Switch->GetComponentLocation();
}

// Called every frame
void ASwitchTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASwitchTest::OnOverlapBegin(
	UPrimitiveComponent *OverlappedComponent, 
	AActor *OtherActor, 
	UPrimitiveComponent *OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult &SweepResult)
{
	if(bCharacterOnSwitch == false)
		bCharacterOnSwitch = true;
	OpenTheDoor();
	SwitchON();
}

void ASwitchTest::OnOverlapEnd(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex)
{
	if(bCharacterOnSwitch == true)
		bCharacterOnSwitch = false;
	GetWorldTimerManager().SetTimer(SwitchTimer, this, &ASwitchTest::CloseDoor, DoorDelayTime);
}

void ASwitchTest::CloseDoor()
{
	if(bCharacterOnSwitch == false){
		CloseTheDoor();
		SwitchOFF();
	}

}



void ASwitchTest::UpdateDoorLocation(float z)
{
	FVector newLocation = InitDoorLoaction;
	newLocation.Z += z;
	Door->SetWorldLocation(newLocation);
}

void ASwitchTest::UpdateSwitchLocation(float z)
{
	FVector newLocation = InitSwitchLocation;
	newLocation.Z += z;
	Switch->SetWorldLocation(newLocation);
}