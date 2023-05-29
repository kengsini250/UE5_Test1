// Fill out your copyright notice in the Description page of Project Settings.


#include "Boom.h"

#include "MainCharacter.h"
#include "NetworkMessage.h"


ABoom::ABoom()
{
}

void ABoom::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if(OtherActor)
	{
		AMainCharacter *mainCharacter = Cast<AMainCharacter>(OtherActor);
		if(mainCharacter)
		{
			mainCharacter->setHP(mainCharacter->HP-Damage);
			Destroy();
		}
	}
}

void ABoom::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
