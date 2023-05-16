// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

#include "MainPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "CharacterTest/MyGameModeBase.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(GetRootComponent());
	CameraArm->TargetArmLength = 400;
	CameraArm->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 500, 0);

	GetCharacterMovement()->JumpZVelocity = 300;
	GetCharacterMovement()->AirControl = 0.75f;

	GameModeT = UGameplayStatics::GetGameMode(GetWorld());
	GameMode = Cast<AMyGameModeBase>(GameModeT);
    		

}

void AMainCharacter::SetMovementStatus(EMovementStatus status)
{
	MovementStatus = status;
	if(MovementStatus == EMovementStatus::EMS_Sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
}

void AMainCharacter::ShiftKeyDown()
{
	bShiftKeyDown = true;
}

void AMainCharacter::ShiftKeyUp()
{
	bShiftKeyDown = false;
}

void AMainCharacter::SetSpStatus(ESpStatus status)
{
	SpStatus = status;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

	float DeltaSp = SpRate*DeltaTime;
	switch(SpStatus)
	{
	case ESpStatus::ESS_Normal:
		if(bShiftKeyDown)
		{
			if(SP - DeltaSp <= SpMinStatus)//低于一定百分比
			{
				SetSpStatus(ESpStatus::ESS_BelowMinimum);//改变状态
				SP -= DeltaSp;
			}
			else
			{
				SP -= DeltaSp;
			}
			SetMovementStatus(EMovementStatus::EMS_Sprinting);
		}
		else//没有按下shift:恢复
		{
			if(SP + DeltaSp >= SP_Max)
			{
				SP = SP_Max;
			}else
			{
				SP += DeltaSp;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;
	case ESpStatus::ESS_BelowMinimum:
		if(bShiftKeyDown)
		{
			if(SP - DeltaSp <= 0)//耗尽
			{
				SetSpStatus(ESpStatus::ESS_Exhausted);
				SP=0;
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}
			else
			{
				SP -= DeltaSp;
				SetMovementStatus(EMovementStatus::EMS_Sprinting);
			}
		}
		else
		{
			if(SP + DeltaSp >= SpMinStatus)
			{
				SP += DeltaSp;
			}else
			{
				SP += DeltaSp;
			}	
			SetMovementStatus(EMovementStatus::EMS_Sprinting);
		}
		break;
	case ESpStatus::ESS_Exhausted:
		if(bShiftKeyDown)
		{
			SP = 0;
		}
		else
		{
			SetSpStatus(ESpStatus::ESS_ExhaustedRecovering);
			SP += DeltaSp;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;
	case ESpStatus::ESS_ExhaustedRecovering:
		if(SP + DeltaSp >= SpMinStatus)
		{
			SetSpStatus(ESpStatus::ESS_Normal);
			SP += DeltaSp;
		}else
		{
			SP+=DeltaSp;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;
	default:
		;
	}
	GameMode->SetSPDelegate.Broadcast(SP/SP_Max);
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump",IE_Released,this,&ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Sprint",IE_Pressed,this,&AMainCharacter::ShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint",IE_Released,this,&AMainCharacter::ShiftKeyUp);

	
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMainCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMainCharacter::LookUpAtRate);
}

void AMainCharacter::MoveForward(float value)
{
	if((Controller!=nullptr)&&(value!=0)){
		const FRotator R = Controller->GetControlRotation();
		const FRotator YawR(0, R.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawR).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, value);

		/*
		//使用委托
		auto GameModeT = UGameplayStatics::GetGameMode(GetWorld());
		auto GameMode = Cast<AMyGameModeBase>(GameModeT);
		if(GameMode)
		{
			//也可以
			// auto CurrController = Cast<AMainPlayerController>(GetController());
			// if(CurrController)
			// {
			// 	SP-=10;
			// 	CurrController->setSP(SP/100.0f);
			// }
		}
		*/
	}
}

void AMainCharacter::MoveRight(float value)
{
	if((Controller!=nullptr)&&(value!=0)){
		const FRotator R = Controller->GetControlRotation();
		const FRotator YawR(0, R.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawR).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, value);
	}
}

void AMainCharacter::TurnAtRate(float value)
{
	AddControllerYawInput(value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::LookUpAtRate(float value)
{
	AddControllerPitchInput(value * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

float AMainCharacter::getCurrHP()
{
	return HP;
}

float AMainCharacter::getCurrHP_Per()
{
	return HP/HP_Max;
}

void AMainCharacter::setHP(float value)
{
	HP = value;
}
