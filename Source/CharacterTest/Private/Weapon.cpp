// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "Enemy.h"
#include "MainCharacter.h"
#include "Chaos/AABBTree.h"
#include "Components/CapsuleComponent.h"
#include "Engine/SkeletalMeshSocket.h"

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	HitCapsule->OnComponentBeginOverlap.AddDynamic(this,&AWeapon::HitCapsuleOnOverlapBegin);
	HitCapsule->OnComponentEndOverlap.AddDynamic(this,&AWeapon::HitCapsuleOnOverlapEnd);
	
	HitCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitCapsule->SetCollisionObjectType(ECC_WorldDynamic);
	HitCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	HitCapsule->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
}

AWeapon::AWeapon()
{
	skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	skeletalMesh->SetupAttachment(GetRootComponent());

	HitCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitCapsule"));
	HitCapsule->SetupAttachment(GetRootComponent());
}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if(OtherActor)
	{
		if(AMainCharacter *mainCharacter = Cast<AMainCharacter>(OtherActor))
		{
			//如果没有武器
			if(mainCharacter->getWeapon() == nullptr)
			{
				//自动捡起武器
				Equip(mainCharacter);
				mainCharacter->setOverlappingItem(nullptr);
			}
			else
			{
				//设置重叠Item
				mainCharacter->setOverlappingItem(this);	
			}
			
    	}	
	}
	
}

void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	
	if(OtherActor)
	{
		if(AMainCharacter *mainCharacter = Cast<AMainCharacter>(OtherActor))
		{
			//清除重叠Item
			mainCharacter->setOverlappingItem(nullptr);
		}	
	}
}

void AWeapon::Equip(AMainCharacter* mainC)
{
	bRotate=false;
	if(mainC)
	{
		skeletalMesh->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
		skeletalMesh->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
		skeletalMesh->SetSimulatePhysics(false);
		if(const USkeletalMeshSocket* WeaponSocket = mainC->GetMesh()->GetSocketByName("WeaponSocket"))
		{
			WeaponSocket->AttachActor(this,mainC->GetMesh());
			mainC->setWeapon(this);
		}
	}
}

void AWeapon::HitCapsuleOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		if(auto Enemy = Cast<AEnemy>(OtherActor))
		{
			const USkeletalMeshSocket* hitParticlesSocket = skeletalMesh->GetSocketByName("HitParticlesSocket");
			if(hitParticlesSocket)
			{
				FVector hitParticlesSocketPos = hitParticlesSocket->GetSocketLocation(skeletalMesh);
				Enemy->BloodParticles(hitParticlesSocketPos);
			}
		}
	}
}

void AWeapon::HitCapsuleOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AWeapon::CollisionON()
{
	HitCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWeapon::CollisionOFF()
{
	HitCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
