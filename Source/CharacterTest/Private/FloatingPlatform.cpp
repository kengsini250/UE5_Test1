// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingPlatform.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

// Sets default values
AFloatingPlatform::AFloatingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();

	StartPoint = GetActorLocation();
	EndPoint += StartPoint;
	Distance = (EndPoint - StartPoint).Size();
	
	GetWorldTimerManager().SetTimer(
		InterpTimer,
		this,
		&AFloatingPlatform::ToggleInterping,
		InterpTime);
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bInterping)
	{
		FVector Curr = GetActorLocation();
		//插值
    	FVector Interp = FMath::VInterpTo(Curr,EndPoint,DeltaTime,Speed);
    	SetActorLocation(Interp);

		float _Distance = (GetActorLocation()-StartPoint).Size();
		if(Distance - _Distance <=0.2f)
		{
			ToggleInterping();
			//重置计时
			GetWorldTimerManager().SetTimer(
				InterpTimer,
				this,
				&AFloatingPlatform::ToggleInterping,
				InterpTime);
			//交换坐标
			auto temp = StartPoint;
			StartPoint = EndPoint;
			EndPoint = temp;
		}
	}


}

void AFloatingPlatform::ToggleInterping()
{
	bInterping = !bInterping;
}

