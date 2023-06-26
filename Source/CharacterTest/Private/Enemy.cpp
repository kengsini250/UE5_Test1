// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "AIController.h"
#include "MainCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("FindEnemyBox"));
	Box->SetupAttachment(GetRootComponent());
	Box->SetBoxExtent(FVector(300,300,100));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	AIController = Cast<AAIController>(GetController());
	Box->OnComponentBeginOverlap.AddDynamic(this,&AEnemy::BoxOnOverlapBegin);
	Box->OnComponentEndOverlap.AddDynamic(this,&AEnemy::BoxOnOverlapEnd);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::MoveToTarget(AMainCharacter* self)
{
	SetMovement(EEnemyMovementStatus::EMS_MoveToTarget);
	UE_LOG(LogTemp,Warning,TEXT("Move To Target"));
}

void AEnemy::BoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		auto self = Cast<AMainCharacter>(OtherActor);
		if(self)
		{
			MoveToTarget(self);
		}
	}
}

void AEnemy::BoxOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

