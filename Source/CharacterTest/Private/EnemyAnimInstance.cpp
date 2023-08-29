// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"

#include "Enemy.h"
#include "Kismet/GameplayStatics.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	if(Pawn == nullptr){
		Pawn = TryGetPawnOwner();
		if(Pawn)
		{
			Enemy = Cast<AEnemy>(Pawn);
		}
	}
}

void UEnemyAnimInstance::Tick()
{
	if(Pawn == nullptr){
		Pawn = TryGetPawnOwner();
	}

	if(Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0);
		MovementSpeed = LateralSpeed.Size();

		if(Enemy == nullptr)
		{
			Enemy = Cast<AEnemy>(Pawn);
		}
	}
}
