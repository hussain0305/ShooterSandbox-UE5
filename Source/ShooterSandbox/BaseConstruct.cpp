// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseConstruct.h"
#include "ShooterSandboxController.h"
#include "ShooterSandboxPlayerState.h"
#include "ShooterSandboxGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "EConstructDestruction.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseConstruct::ABaseConstruct()
{
	SetReplicates(true);

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Construct Root"));
	SetRootComponent(rootComp);

	baseOrientation = CreateDefaultSubobject<USceneComponent>(TEXT("Base Orientation"));
	baseOrientation->SetupAttachment(GetRootComponent());

	constructBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Construct Base"));
	constructBase->SetupAttachment(baseOrientation);

	constructBaseGeometry = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Construct Base Geometry"));
	constructBaseGeometry->SetupAttachment(baseOrientation);
}

// Called when the game starts or when spawned
void ABaseConstruct::BeginPlay()
{
	Super::BeginPlay();

	health = maxHealth;

	ConstructFormation();
	BP_SetupDestruction();
	FetchConstructDetailsFromDatabase();
}

float ABaseConstruct::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!HasAuthority() || DamageCauser == nullptr || EventInstigator == nullptr)
	{
		return health;
	}

	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	health -= DamageAmount;
	Cast<AShooterSandboxController>(EventInstigator)->ProxyForHUD_ByCommandCode(EHUDCommandType::SuccessfulHit);

	if (health < 0 && !destroyed)
	{
		Cast<AShooterSandboxPlayerState>(EventInstigator->PlayerState)->HasBrokenConstruct(maxHealth);
		Cast<AShooterSandboxController>(EventInstigator)->ProxyForHUD_ByCommandCode(EHUDCommandType::PlayHitAnimation);
		int lmh = DamageAmount < 50 ? 1 : DamageAmount < 100 ? 2 : 3;
		DestroyConstruct(lmh);
	}

	return health;
}

void ABaseConstruct::SetConstructedBy(AShooterSandboxController* owner)
{
	constructedBy = owner;
}

AShooterSandboxController* ABaseConstruct::GetConstructedBy()
{
	return constructedBy;
}

void ABaseConstruct::DestroyConstruct(int lowMidHigh)
{
	destroyed = true;
	//!UPGRADE TODO
	if (GetLocalRole() < ROLE_Authority)
	{
		return;
	}

	if (destructionBP)
	{
		Multicast_DestroyConstruct(lowMidHigh);
	}

	//Probably resolved below. Ignore "!UPGRADE TODO"
	//!UPGRADE TODO
	//Destroy();
}

void ABaseConstruct::Multicast_DestroyConstruct_Implementation(int lowMidHigh)
{
	//BP_BlockifyConstruct();
	destroyed = true;

	if (constructComposition == EConstructComposition::DestructibleGeometry)
	{
		constructBaseGeometry->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		constructBaseGeometry->SetSimulatePhysics(true);
		constructBaseGeometry->SetEnableGravity(true);
		BP_BlockifyConstruct();
		return;
	}

	//TArray<UStaticMeshComponent*> allStaticMeshes;
	//Cast<AActor>(this)->GetComponents<UStaticMeshComponent>(allStaticMeshes);

	//FActorSpawnParameters spawnParams;
	//spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//AEConstructDestruction* spawnedDestruction = GetWorld()->SpawnActor<AEConstructDestruction>(
	//	destructionBP, GetActorLocation() + allStaticMeshes[0]->GetRelativeLocation(), GetActorRotation(), spawnParams);

	//if (spawnedDestruction != nullptr)
	//{
	//	spawnedDestruction->DestroyConstruct(Cast<UMaterialInstance>(destructionMaterials[appearanceIndex]), lowMidHigh);
	//}
	DestroyNow();
}

void ABaseConstruct::DestroyNow()
{
	Destroy();
}

void ABaseConstruct::ConstructFormation()
{
	if (constructComposition == EConstructComposition::DestructibleMesh || constructComposition == EConstructComposition::DestructibleGeometry)
	{
		constructBaseGeometry->SetSimulatePhysics(false);
		constructBaseGeometry->SetEnableGravity(false);
		constructBaseGeometry->SetActive(false);
		constructBaseGeometry->SetVisibility(false, true);
	}

	//!UPGRADE TODO
	if (shouldForm && GetLocalRole() == ROLE_Authority)
	{
		FormationScaling(FVector(0, 0, 0), Cast<UStaticMeshComponent>(constructBase)->GetComponentScale(), 0.0f);
	}

	if (appearanceOptions.Num() == 0)
	{
		appearanceIndex = 0;
		return;
	}

	appearanceIndex = FMath::RandRange(0, appearanceOptions.Num() - 1);

	TArray<UActorComponent*> mainMesh;
	mainMesh = Cast<AActor>(this)->GetComponentsByClass(UStaticMeshComponent::StaticClass());

	if (mainMesh.Num() != 0)
	{
		if (Cast<UStaticMeshComponent>(mainMesh[0]))
		{
			Cast<UStaticMeshComponent>(mainMesh[0])->SetMaterial(0, appearanceOptions[appearanceIndex]);
		}

		return;
	}

	TArray<UStaticMeshComponent*> allStaticMeshes;
	Cast<AActor>(this)->GetComponents<UStaticMeshComponent>(allStaticMeshes);

	if (allStaticMeshes.Num() == 0 || appearanceOptions.Num() == 0)
	{
		return;
	}

	allStaticMeshes[0]->SetMaterial(0, appearanceOptions[appearanceIndex]);
}

void ABaseConstruct::FetchConstructDetailsFromDatabase()
{
	FConstructsDatabase* databaseRow;
	if (GetWorld()->GetAuthGameMode<AShooterSandboxGameMode>()->GetConstructDetails(FName(*constructRowName), databaseRow))
	{
		constructName = databaseRow->displayName;
		constructionCost = databaseRow->constructionCost;
	}
}

void ABaseConstruct::FormationScaling(FVector current, FVector fullScale, float alpha)
{
	Cast<UStaticMeshComponent>(constructBase)->SetWorldScale3D(FMath::Lerp(current, fullScale, alpha));
	//Cast<AActor>(this)->SetActorScale3D(FMath::Lerp(current, fullScale, alpha));

	if (alpha < 1)
	{
		FTimerHandle scalingUp;
		FTimerDelegate scalingDelegate;

		scalingDelegate.BindUFunction(this, FName("FormationScaling"), Cast<UStaticMeshComponent>(constructBase)->GetComponentScale(), fullScale, alpha + 0.05f);
		GetWorld()->GetTimerManager().SetTimer(scalingUp, scalingDelegate, 0.025f, false);
	}
	else
	{
		switch (constructComposition)
		{
			case EConstructComposition::DestructibleGeometry:
			case EConstructComposition::IndestructibleGeometry:
			{
				constructBase->SetActive(false);
				constructBase->SetVisibility(false, true);
				constructBase->DestroyComponent();

				//constructBaseGeometry->Activate();
				constructBaseGeometry->SetActive(true);
				constructBaseGeometry->SetVisibility(true, true);

				constructBaseGeometry->SetSimulatePhysics(true);
				constructBaseGeometry->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				constructBaseGeometry->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
				constructBaseGeometry->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel4);
				GEngine->AddOnScreenDebugMessage(1, 15.0f, FColor::Yellow, TEXT("SWITCHED!!!!!!"));

				TArray<UActorComponent*> mainMesh;
				mainMesh = Cast<AActor>(this)->GetComponentsByClass(UGeometryCollectionComponent::StaticClass());

				if (mainMesh.Num() != 0)
				{
					if (Cast<UGeometryCollectionComponent>(mainMesh[0]))
					{
						Cast<UGeometryCollectionComponent>(mainMesh[0])->SetMaterial(0, appearanceOptions[appearanceIndex]);
					}

					return;
				}

				TArray<UGeometryCollectionComponent*> allGeometryColelctions;
				Cast<AActor>(this)->GetComponents<UGeometryCollectionComponent>(allGeometryColelctions);

				if (allGeometryColelctions.Num() == 0 || appearanceOptions.Num() == 0)
				{
					return;
				}

				allGeometryColelctions[0]->SetMaterial(0, appearanceOptions[appearanceIndex]);
		}
		break;
		}
	}
}

void ABaseConstruct::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseConstruct, health);
	DOREPLIFETIME(ABaseConstruct, constructedBy);
}