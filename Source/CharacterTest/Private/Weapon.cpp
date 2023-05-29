// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "MainCharacter.h"
#include "Engine/SkeletalMeshSocket.h"

AWeapon::AWeapon()
{
	skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	skeletalMesh->SetupAttachment(GetRootComponent()); 
}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if(OtherActor)
	{
		if(AMainCharacter *mainCharacter = Cast<AMainCharacter>(OtherActor))
		{
			Equip(mainCharacter);
    	}	
	}
	
}

void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::Equip(AMainCharacter* mainC)
{
	if(mainC)
	{
		skeletalMesh->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
		skeletalMesh->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
		skeletalMesh->SetSimulatePhysics(false);
		if(const USkeletalMeshSocket* WeaponSocket = mainC->GetMesh()->GetSocketByName("WeaponSocket"))
		{
			WeaponSocket->AttachActor(this,mainC->GetMesh());
		}
	}
}
