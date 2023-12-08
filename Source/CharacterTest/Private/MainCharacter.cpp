// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

#include "Enemy.h"
#include "MainPlayerController.h"
#include "SaveGame_NoBP.h"
#include "SaveWeapon_BP.h"
#include "Weapon.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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

	// GameModeT = UGameplayStatics::GetGameMode(GetWorld());
	// GameMode = Cast<AMyGameModeBase>(GameModeT);
}

void AMainCharacter::SetMovementStatus(EMovementStatus status)
{
	MovementStatus = status;
	if (MovementStatus == EMovementStatus::EMS_Sprinting)
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

void AMainCharacter::G_Up()
{
	bGiveWeapon = false;
}

void AMainCharacter::G_Down()
{
	if(MovementStatus == EMovementStatus::EMS_Dead) return;
	if(mainController)
	{
		if(mainController->bPauseMenu)
			return;
	}
	
	bGiveWeapon = true;
	
	//手里没有武器
	if(Weapon == nullptr)
	{
		if (OverlappingItem)
		{
			if (auto weapon = Cast<AWeapon>(OverlappingItem))
			{
				weapon->Equip(this);
				OverlappingItem = nullptr;
			}
		}
	}
	//手里有武器
	else
	{
		//清空手持武器
		Weapon->Destroy();
		Weapon = nullptr;
		
		//与武器重叠
		if (OverlappingItem)
		{
			if (auto weapon = Cast<AWeapon>(OverlappingItem))
			{
				weapon->Equip(this);
				setOverlappingItem(nullptr);
			}
		}
	}
}

void AMainCharacter::LMBUp()
{
	bLMBDown = false;
}

void AMainCharacter::LMBDown()
{
	if(mainController)
	{
		if(mainController->bPauseMenu)
			return;
	}
	
	if(Weapon == nullptr)
	{
		//没武器
	}
	else
	{
		bLMBDown = true;
		//有武器
		Attack();
	}
}

void AMainCharacter::Attack()
{
	if(!bAttacking && (MovementStatus != EMovementStatus::EMS_Dead))
	{
		bAttacking = true;
		SetInterpToEnemy(true);

		
		UAnimInstance*AnimInstance=GetMesh()->GetAnimInstance();
		if(AnimInstance)
		{
			//随机播放动画
			int32 mode = FMath::RandRange(0,1);
			switch(mode)
			{
			case 0:
				AnimInstance->Montage_Play(AttackMontage,2.0f);
				AnimInstance->Montage_JumpToSection(FName("Attack1"),AttackMontage);
				break;
			case 1:
				AnimInstance->Montage_Play(AttackMontage,1.5f);
				AnimInstance->Montage_JumpToSection(FName("Attack2"),AttackMontage);
				break;
			default:
					;
			}

		}
	}

}

void AMainCharacter::Die()
{
	//被多个敌人攻击后，只播放一次死亡动画
	if(MovementStatus != EMovementStatus::EMS_Dead)
	{
		UAnimInstance*AnimInstance=GetMesh()->GetAnimInstance();
		if(AnimInstance)
		{
			AnimInstance->Montage_Play(AttackMontage,2.0f);
			AnimInstance->Montage_JumpToSection(FName("Death"),AttackMontage);
		}
	}
	SetMovementStatus(EMovementStatus::EMS_Dead);
}

void AMainCharacter::DeathEnd()
{
	GetMesh()->bPauseAnims=true;
	GetMesh()->bNoSkeletonUpdate = true;
}

void AMainCharacter::Jump()
{
	if(mainController)
	{
		if(mainController->bPauseMenu)
			return;
	}	
	if(MovementStatus != EMovementStatus::EMS_Dead)
	{
		Super::Jump();
	}
}

void AMainCharacter::AttackEnd()
{
	bAttacking = false;
	SetInterpToEnemy(false);
	//连续攻击
	if(bLMBDown)
	{
		Attack();
	}
}

void AMainCharacter::SetSpStatus(ESpStatus status)
{
	SpStatus = status;
}

void AMainCharacter::SetInterpToEnemy(bool Interp)
{
	bInterpToEnemy = Interp;
}

FRotator AMainCharacter::GetLookAtRotatorYaw(FVector target)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),target);
	FRotator LookAtRotationYaw(0,LookAtRotation.Yaw,0);
	return LookAtRotationYaw;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	mainController = Cast<AMainPlayerController>(GetController());
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//如果死亡，直接跳过计算
	if(MovementStatus == EMovementStatus::EMS_Dead)
		return;

	float DeltaSp = SpRate * DeltaTime;
	switch (SpStatus)
	{
	case ESpStatus::ESS_Normal:
		if (bShiftKeyDown)
		{
			if (SP - DeltaSp <= SpMinStatus) //低于一定百分比
			{
				SetSpStatus(ESpStatus::ESS_BelowMinimum); //改变状态
				SP -= DeltaSp;
			}
			else
			{
				SP -= DeltaSp;
			}
			SetMovementStatus(EMovementStatus::EMS_Sprinting);
		}
		else //没有按下shift:恢复
		{
			if (SP + DeltaSp >= SP_Max)
			{
				SP = SP_Max;
			}
			else
			{
				SP += DeltaSp;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;
	case ESpStatus::ESS_BelowMinimum:
		if (bShiftKeyDown)
		{
			if (SP - DeltaSp <= 0) //耗尽
			{
				SetSpStatus(ESpStatus::ESS_Exhausted);
				SP = 0;
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
			if (SP + DeltaSp >= SpMinStatus)
			{
				SP += DeltaSp;
			}
			else
			{
				SP += DeltaSp;
			}
			SetMovementStatus(EMovementStatus::EMS_Sprinting);
		}
		break;
	case ESpStatus::ESS_Exhausted:
		if (bShiftKeyDown)
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
		if (SP + DeltaSp >= SpMinStatus)
		{
			SetSpStatus(ESpStatus::ESS_Normal);
			SP += DeltaSp;
		}
		else
		{
			SP += DeltaSp;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;
	default:
		;
	}
	
	if(mainController)
	{
		mainController->setSP(SP/SP_Max);	
	}

	if(bInterpToEnemy && InterpTarget)
	{
		FRotator LookAtYaw = GetLookAtRotatorYaw(InterpTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(),LookAtYaw,DeltaTime,InterpSpeed);
		SetActorRotation(InterpRotation);
	}
	if(InterpTarget)
	{
		EnemyTargetLocation = InterpTarget->GetActorLocation();
		//敌人坐标传递给control用来绘制UI
		if(mainController)
		{
			mainController->EnemyLocation = EnemyTargetLocation;
		}
	}
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMainCharacter::ShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMainCharacter::ShiftKeyUp);

	PlayerInputComponent->BindAction("GiveWeapon", IE_Pressed, this, &AMainCharacter::G_Down);
	PlayerInputComponent->BindAction("GiveWeapon", IE_Released, this, &AMainCharacter::G_Up);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMainCharacter::LMBDown);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &AMainCharacter::LMBUp);
	PlayerInputComponent->BindAction("ESC", IE_Pressed, this, &AMainCharacter::EscDown);
	PlayerInputComponent->BindAction("ESC", IE_Released, this, &AMainCharacter::EscUp);
	
	
	PlayerInputComponent->BindAxis("Turn", this, &AMainCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMainCharacter::LookUp);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMainCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMainCharacter::LookUpAtRate);
}

float AMainCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                 AActor* DamageCauser)
{
	setHP(getCurrHP()-Damage);
	if(getCurrHP()<=0)
	{
		Die();
	}
	return Damage;
}

void AMainCharacter::MoveForward(float value)
{
	if ((Controller != nullptr)
		&& (value != 0)
		&& (!bAttacking)
		&& (MovementStatus != EMovementStatus::EMS_Dead)
		&& !mainController->bPauseMenu)
	{
		const FRotator R = Controller->GetControlRotation();
		const FRotator YawR(0, R.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawR).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, value);
	}
}

void AMainCharacter::MoveRight(float value)
{
	if ((Controller != nullptr)
		&& (value != 0)
		&&(!bAttacking)
		&&
		(MovementStatus != EMovementStatus::EMS_Dead)
		&& !mainController->bPauseMenu)
	{
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

void AMainCharacter::LookUp(float value)
{
	if ((Controller != nullptr)
	&& (value != 0)
	&& (!bAttacking)
	&& (MovementStatus != EMovementStatus::EMS_Dead)
	&& !mainController->bPauseMenu)
	{
		AddControllerPitchInput(value);
	}
}

void AMainCharacter::Turn(float value)
{
	if ((Controller != nullptr)
	&& (value != 0)
	&& (!bAttacking)
	&& (MovementStatus != EMovementStatus::EMS_Dead)
	&& !mainController->bPauseMenu)
	{
		AddControllerYawInput(value);
	}
}

float AMainCharacter::getCurrHP()
{
	return HP;
}

float AMainCharacter::getCurrHP_Per()
{
	return HP / HP_Max;
}

void AMainCharacter::setHP(float value)
{
	HP = value;
}

void AMainCharacter::switchLevel(FName name)
{
	UWorld* World = GetWorld();
	if(World)
	{
		FString CurrLevel = World->GetMapName();
		FName CurrLevelName(*CurrLevel);
		if(CurrLevelName!=name)
		{
			UGameplayStatics::OpenLevel(World,name);
		}
	}
}

void AMainCharacter::SaveGame()
{
	USaveGame_NoBP* SaveGameInstance = 
	Cast<USaveGame_NoBP>(
	UGameplayStatics::CreateSaveGameObject(
		USaveGame_NoBP::StaticClass())
		);

	SaveGameInstance->characterState.HP = HP;
	SaveGameInstance->characterState.HP_Max = HP_Max;
	SaveGameInstance->characterState.SP = SP;
	SaveGameInstance->characterState.SP_Max = SP_Max;
	SaveGameInstance->characterState.Location = GetActorLocation();
	SaveGameInstance->characterState.Rotator = GetActorRotation();

	if(Weapon)
	{
		SaveGameInstance->characterState.WeaponName = Weapon->WeaponName;
	}

	UGameplayStatics::SaveGameToSlot(
		SaveGameInstance,
		SaveGameInstance->SaveName,
		SaveGameInstance->SaveID);
	
}

void AMainCharacter::LoadGame(bool SetPos)
{
	USaveGame_NoBP* LoadGameInstance = 
	Cast<USaveGame_NoBP>(
UGameplayStatics::CreateSaveGameObject(
	USaveGame_NoBP::StaticClass())
	);

	LoadGameInstance = Cast<USaveGame_NoBP>(
	UGameplayStatics::LoadGameFromSlot(
		LoadGameInstance->SaveName,
		LoadGameInstance->SaveID)
		);

	HP = LoadGameInstance->characterState.HP;
	HP_Max = LoadGameInstance->characterState.HP_Max;
	SP = LoadGameInstance->characterState.SP;
	SP_Max = LoadGameInstance->characterState.SP_Max;

	if(SaveWeaponMap)
	{
		ASaveWeapon_BP* savedWeapon = GetWorld()->SpawnActor<ASaveWeapon_BP>(SaveWeaponMap);
		if(savedWeapon)
		{
			FString name = LoadGameInstance->characterState.WeaponName;
			if(savedWeapon->WeaponMap.Contains(name))
			{
				AWeapon* ToEquip = GetWorld()->SpawnActor<AWeapon>(savedWeapon->WeaponMap[name]);
				if(Weapon)
				{
					Weapon->Destroy();
					Weapon = nullptr;
				}
				
				ToEquip->Equip(this);
			}

		}
	}
	

	if(SetPos)
	{
		SetActorLocation(LoadGameInstance->characterState.Location);
		SetActorRotation(LoadGameInstance->characterState.Rotator);
	}
	//改变角色状态
	SetMovementStatus(EMovementStatus::EMS_Normal);
	//恢复动画
	GetMesh()->bPauseAnims=false;
	GetMesh()->bNoSkeletonUpdate = false;
}

void AMainCharacter::EscUp()
{
	bESC = false;
}

void AMainCharacter::EscDown()
{
	bESC = true;
	if(mainController)
	{
		mainController->TogglePauseMenu();
	}
}
