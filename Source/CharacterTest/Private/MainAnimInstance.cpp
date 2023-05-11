// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"

#include "MainCharacter.h"
#include "GameFramework/PawnMovementComponent.h"


void UMainAnimInstance::NativeInitializeAnimation()
{
    if(Pawn == nullptr){
        Pawn = TryGetPawnOwner();
        if(Pawn)
        {
            MainCharacter = Cast<AMainCharacter>(Pawn);
        }
    }
}

void UMainAnimInstance::Tick()
{
    if(Pawn == nullptr){
        Pawn = TryGetPawnOwner();
    }

    if(Pawn)
    {
        FVector Speed = Pawn->GetVelocity();
        FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0);
        MovementSpeed = LateralSpeed.Size();
        bIsInAir = Pawn->GetMovementComponent()->IsFalling();

        if(MainCharacter == nullptr)
        {
            MainCharacter = Cast<AMainCharacter>(Pawn);
        }
    }
}
