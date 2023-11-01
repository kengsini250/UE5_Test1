// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "AIController.h"
#include "MainCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
#include "Sound/SoundCue.h"
#include "Components/SceneComponent.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("FindEnemyBox"));
	Box->SetupAttachment(GetRootComponent());
	Box->SetBoxExtent(FVector(300, 300, 100));

	HitCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitCapsule"));
	HitCapsule->SetupAttachment(GetRootComponent());
	HitCapsule->SetCapsuleRadius(70);
	HitCapsule->SetCapsuleHalfHeight(110);

	WeaponCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitCollision"));
	WeaponCollision->SetupAttachment(GetMesh(), FName("EnemyWeaponSlot"));

	GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
}

void AEnemy::BloodParticles(const FVector& pos)
{
	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			pos,
			FRotator(0),
			false);
	}
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	AIController = Cast<AAIController>(GetController());
	Box->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::BoxOnOverlapBegin);
	Box->OnComponentEndOverlap.AddDynamic(this, &AEnemy::BoxOnOverlapEnd);
	HitCapsule->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::HitCapsuleOnOverlapBegin);
	HitCapsule->OnComponentEndOverlap.AddDynamic(this, &AEnemy::HitCapsuleOnOverlapEnd);
	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::WeaponOnOverlapBegin);
	WeaponCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemy::WeaponOnOverlapEnd);

	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollision->SetCollisionObjectType(ECC_WorldDynamic);
	WeaponCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
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

void AEnemy::MoveToTarget(AMainCharacter* s)
{
	SetMovement(EEnemyMovementStatus::EMS_MoveToTarget);
	if (AIController)
	{
		FAIMoveRequest re;
		re.SetGoalActor(s);
		re.SetAcceptanceRadius(10.0f);
		FNavPathSharedPtr ptr;

		AIController->MoveTo(re, &ptr);
	}
}

void AEnemy::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(AttackMontage, 2.0f);
		AnimInstance->Montage_JumpToSection(FName("Death"), AttackMontage);
	}
	SetMovement(EEnemyMovementStatus::EMS_Death);

	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

float AEnemy::TakeDamage(float _Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
                         AActor* DamageCauser)
{
	if(HP - _Damage <= 0)
	{
		HP-=_Damage;
		Die();
	}
	else
	{
		HP-=_Damage;
	}

	return _Damage;
}

void AEnemy::BoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                               const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		auto mainCharacter = Cast<AMainCharacter>(OtherActor);
		if (mainCharacter)
		{
			MoveToTarget(mainCharacter);
		}
	}
}

void AEnemy::BoxOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		if (auto mainCharactor = Cast<AMainCharacter>(OtherActor))
		{
			SetMovement(EEnemyMovementStatus::EMS_Idle);
			AIController->StopMovement();
		}
	}
}

void AEnemy::HitCapsuleOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (auto mainCharactor = Cast<AMainCharacter>(OtherActor))
		{
			mainCharactor->SetInterpTarget(this);
			HitTarget = mainCharactor;
			bOverlappingHitCapsule = true;
			Attack();
		}
	}
}

void AEnemy::HitCapsuleOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		if (auto mainCharactor = Cast<AMainCharacter>(OtherActor))
		{
			if (mainCharactor->InterpTarget == this)
			{
				mainCharactor->SetInterpTarget(nullptr);
			}
			bOverlappingHitCapsule = false;
			SetMovement(EEnemyMovementStatus::EMS_Idle);
			MoveToTarget(mainCharactor);
			HitTarget = nullptr;
			GetWorldTimerManager().ClearTimer(AttackTimer);
		}
	}
}

void AEnemy::WeaponOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (auto mainC = Cast<AMainCharacter>(OtherActor))
		{
			if (mainC->HitParticles)
			{
				const USkeletalMeshSocket* EnemyWeaponSlot = GetMesh()->GetSocketByName("EnemyWeaponSlot");
				if (EnemyWeaponSlot)
				{
					FVector Pos = EnemyWeaponSlot->GetSocketLocation(GetMesh());
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), mainC->HitParticles, Pos, FRotator(0), false);
				}
			}
			if (mainC->HitSound)
			{
				UGameplayStatics::PlaySound2D(this, mainC->HitSound);
			}
			if(DamageTypeClass)
			{
				UGameplayStatics::ApplyDamage(mainC,Damage,AIController,this,DamageTypeClass);
			}
		}
	}
}

void AEnemy::WeaponOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AEnemy::AttackCollisionStart()
{
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	UGameplayStatics::PlaySound2D(this, SwingSound);
}

void AEnemy::AttackCollisionEnd()
{
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::Attack()
{
	bAttacking = false;
	if (AIController)
	{
		AIController->StopMovement();
		SetMovement(EEnemyMovementStatus::EMS_Attacking);
	}
	if (!bAttacking)
	{
		bAttacking = true;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(AttackMontage, 2.0f);
			AnimInstance->Montage_JumpToSection(FName("Attack"), AttackMontage);
		}
	}
	if (SwingSound)
	{
		UGameplayStatics::PlaySound2D(this, SwingSound);
	}
}

void AEnemy::EnemyAttackEnd()
{
	bAttacking = false;
	if (bOverlappingHitCapsule)
	{
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
	}
}
