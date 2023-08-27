// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterSandboxGlobal.h"
#include "BaseConstruct.generated.h"

UCLASS()
class SHOOTERSANDBOX_API ABaseConstruct : public APawn
{
	GENERATED_BODY()

public:

	ABaseConstruct();

	/**************************
	*       VARIABLES         *
	**************************/

protected:

	//=#=#=#=#= EDITABLE IN BLUEPRINTS =#=#=#=#=

	int appearanceIndex;
	bool destroyed = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int maxHealth;

	UPROPERTY(Replicated)
		int health;

	UPROPERTY(Replicated)
		class AShooterSandboxController* constructedBy;

public:

	//=#=#=#=#= EDITABLE IN BLUEPRINTS =#=#=#=#=

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EConstructComposition constructComposition; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recoil")
		class USceneComponent* rootComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		USceneComponent* baseOrientation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UStaticMeshComponent* constructBase;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UGeometryCollectionComponent* constructBaseGeometry;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isGridAligned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool canBeParented;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString constructName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int constructionCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString constructRowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float fadeOutDuration = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Materials)
		TArray<class UMaterialInstance*> appearanceOptions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Materials)
		TArray<class UMaterialInstance*> destructionMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool shouldForm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destruction")
		TSubclassOf<class AEConstructDestruction> destructionBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
		class UMaterialInstanceDynamic* createdDynamicMaterial;

	/**************************
	*       FUNCTIONS         *
	**************************/

	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void SetConstructedBy(class AShooterSandboxController* owner);

	class AShooterSandboxController* GetConstructedBy();

	virtual void DestroyConstruct(int lowMidHigh);

	UFUNCTION(BlueprintCallable, Category = "Materials")
		void ConstructFormation();

	UFUNCTION(BlueprintCallable, Category = "Construct Details")
		void FetchConstructDetailsFromDatabase();

	UFUNCTION(Category = "Materials")
		void FormationScaling(FVector current, FVector fullScale, float alpha);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Destruction")
		void BP_SetupDestruction();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Destruction")
		void BP_BlockifyConstruct();

	UFUNCTION(BlueprintCallable, Category = "Destruction")
		void DestroyNow();

	//=#=#=#=#= SERVER FUNCTIONS =#=#=#=#=

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_DestroyConstruct(int lowMidHigh);
};
