// Fill out your copyright notice in the Description page of Project Settings.

#include "P38_GM.h"
#include "P38Pawn.h"
#include "P38_PC.h"

AP38_GM::AP38_GM()
{
	DefaultPawnClass = AP38Pawn::StaticClass();
	PlayerControllerClass = AP38_PC::StaticClass();
}


