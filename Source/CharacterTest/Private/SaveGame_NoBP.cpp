// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGame_NoBP.h"

USaveGame_NoBP::USaveGame_NoBP()
{
	SaveName = TEXT("Default");
	SaveID = 0;
	characterState.WeaponName=TEXT("");
	characterState.MapName=TEXT("");
}
