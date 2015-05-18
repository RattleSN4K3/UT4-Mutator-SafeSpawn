#pragma once

#include "Core.h"
#include "Engine.h"
//#include "UnrealTournament.h"
#include "UTMutator.h"
#include "UTCharacter.h"
//#include "UTTimedPowerup.h"
//#include "UTPlayerController.h"

#include "SafeSpawnRepInfo.h"
#include "SafeSpawnLink.h"
#include "SafeSpawnInventory.h"

#include "SafeSpawnMutator.generated.h"


UCLASS(Blueprintable, Config = SafeSpawn)
class ASafeSpawnMutator : public AUTMutator
{
	GENERATED_UCLASS_BODY()

	//'''''''''''''''''''''''''
	// Config
	//'''''''''''''''''''''''''

	UPROPERTY(BlueprintReadWrite, Config)
	float GhostProtectionTime;

	UPROPERTY(BlueprintReadWrite, Config)
	float InitialFireDelay;

	//'''''''''''''''''''''''''
	// Workflow variables
	//'''''''''''''''''''''''''

	UPROPERTY()
	USoundBase* InventoryWarningSound;
	UPROPERTY()
	USoundBase* InventoryTimeoutSound;

	UPROPERTY()
	USoundBase* RestoreSpawnSound;
	UPROPERTY()
	TSubclassOf<class AUTReplicatedEmitter> RestoreTeleportEffect;

	UPROPERTY()
	USoundBase* NoSound;

	//'''''''''''''''''''''''''
	// Classes
	//'''''''''''''''''''''''''

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<ASafeSpawnInventory> DamageHelperClass;

	//'''''''''''''''''''''''''
	// Inherited funtions
	//'''''''''''''''''''''''''

	virtual void BeginPlay() override;

	void PostPlayerInit_Implementation(AController* C) override;
	void NotifyLogout_Implementation(AController* C) override;

	void ModifyPlayer_Implementation(APawn* Other, bool bIsNewSpawn) override;
	bool CheckRelevance_Implementation(AActor* Other) override;

	// TODO: Add Vehicle support
	//bool DriverEnteredVehicle_Implementation(AVehicle* V, APawn* P) override;


	//'''''''''''''''''''''''''
	// Inherited GameRules functions
	//'''''''''''''''''''''''''

	bool ModifyDamage_Implementation(UPARAM(ref) int32& Damage, UPARAM(ref) FVector& Momentum, APawn* Injured, AController* InstigatedBy, const FHitResult& HitInfo, AActor* DamageCauser, TSubclassOf<UDamageType> DamageType) override;
	bool OverridePickupQuery_Implementation(APawn* Other, TSubclassOf<AUTInventory> ItemClass, AActor* Pickup, bool& bAllowPickup) override;


	//'''''''''''''''''''''''''
	// Delegate Callbacks
	//'''''''''''''''''''''''''

	void OnUnProtectFire(APlayerController* PC, ASafeSpawnRepInfo* ClientRI);
	void OnUnProtectPickup(ACharacter* P);


	//'''''''''''''''''''''''''
	// Private funtions
	//'''''''''''''''''''''''''

	void ConditionallyAddPRIFor(APlayerController* PC);
	void RemovePRIFrom(APlayerController* PC);

	void ConditionallyAddLinkedRIFor(AController* C);
	void RemoveLinkedRIFrom(AController* C);

	void ProtectPlayer(AUTCharacter* Other, bool bProtect)
	{
		ASafeSpawnRepInfo* ClientRI = NULL;
		ProtectPlayer(Other, bProtect, ClientRI);
	}

	void ProtectPlayer(AUTCharacter* Other, bool bProtect, ASafeSpawnRepInfo*& ClientRI);

	bool GiveInventory(AUTCharacter* Other);

	//'''''''''''''''''''''''''
	// Helper
	//'''''''''''''''''''''''''

	// TODO: implement helpers in header?!

	bool HasInventory(AUTCharacter* Other);
	bool HasInventory(AUTCharacter* Other, AUTInventory*& inv);
	bool GetPC(ACharacter* P, APlayerController*& PC);
	AController* GetController(ACharacter* P);
	bool GetClientRI(APlayerController* PC, ASafeSpawnRepInfo*& out_ClientRI);
	bool GetLinkedRI(AController* C, ASafeSpawnLink*& out_LinkedRI);
};