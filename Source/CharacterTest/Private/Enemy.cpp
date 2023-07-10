// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "AIController.h"
#include "MainCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("FindEnemyBox"));
	Box->SetupAttachment(GetRootComponent());
	Box->SetBoxExtent(FVector(300,300,100));

	HitCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitCapsule"));
	HitCapsule->SetupAttachment(GetRootComponent());
	HitCapsule->SetCapsuleRadius(70);
	HitCapsule->SetCapsuleHalfHeight(110);

	GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
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
	if(AIController)
	{
		FAIMoveRequest re;
		re.SetGoalActor(self);
		re.SetAcceptanceRadius(50.0f);
		FNavPathSharedPtr ptr;

		AIController->MoveTo(re,&ptr);
	}
}

void AEnemy::BoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		auto mainCharacter = Cast<AMainCharacter>(OtherActor);
		if(mainCharacter)
		{
			MoveToTarget(mainCharacter);
		}
	}
}

void AEnemy::BoxOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

