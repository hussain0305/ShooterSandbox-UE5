// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"

ATower::ATower()
{
	towerBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tower Base"));
	towerBase->SetupAttachment(GetRootComponent());
}
