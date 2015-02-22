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

	void ModifyPlayer_Implementation(APawn* Other) override;
	bool CheckRelevance_Implementation(AActor* Other) override;

	// TODO: Add Vehicle support
	//bool DriverEnteredVehicle_Implementation(AVehicle* V, APawn* P) override;


	//'''''''''''''''''''''''''
	// Inherited GameRules functions
	//'''''''''''''''''''''''''

	void ModifyDamage_Implementation(int32& Damage, FVector& Momentum, APawn* Injured, AController* InstigatedBy, const FHitResult& HitInfo, AActor* DamageCauser, TSubclassOf<UDamageType> DamageType) override;
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

	inline void ProtectPlayer(AUTCharacter* Other, bool bProtect)
	{
		ASafeSpawnRepInfo* ClientRI;
		ProtectPlayer(Other, bProtect, ClientRI);
	}

	void ProtectPlayer(AUTCharacter* Other, bool bProtect, ASafeSpawnRepInfo*& ClientRI);

	bool GiveInventory(AUTCharacter* Other);

	//'''''''''''''''''''''''''
	// Helper
	//'''''''''''''''''''''''''

	inline bool HasInventory(AUTCharacter* Other)
	{
		AUTInventory* inv;
		return HasInventory(Other, inv);
	}

	inline bool HasInventory(AUTCharacter* Other, AUTInventory*& inv)
	{
		inv = Other->FindInventoryType<AUTInventory>(DamageHelperClass, true);
		return (inv != NULL);
	}

	inline bool GetPC(ACharacter* P, APlayerController*& PC) const
	{
		if (P == NULL)
			return false;

		// TODO: Add Vehicle support
		//PC = Cast<AUTPlayerController>(P->Controller);
		//if (PC == NULL && P->DrivenVehicle != NULL)
		//{
		//	PC = UTPlayerController(P->DrivenVehicle->Controller);
		//}

		return PC != NULL;
	}

	inline AController* GetController(ACharacter* P) const
	{
		if (P == NULL)
			return NULL;

		AController* C = P->Controller;
		// TODO: Add Vehicle support
		//if (C == NULL && P->DrivenVehicle != NULL)
		//{
		//	C = P->DrivenVehicle->Controller;
		//}

		return C;
	}

	inline bool GetClientRI(APlayerController* PC, ASafeSpawnRepInfo*& out_ClientRI) const
	{
		for (auto Child : PC->Children)
		{
			ASafeSpawnRepInfo* Rep = Cast<ASafeSpawnRepInfo>(Child);
			if (Rep != NULL)
			{
				out_ClientRI = Rep;
				break;
			}
		}

		return out_ClientRI != NULL;
	}

	inline bool GetLinkedRI(AController* C, ASafeSpawnLink*& out_LinkedRI) const
	{
		if (C != NULL)
		{
			for (auto Child : C->Children)
			{
				ASafeSpawnLink* Link = Cast<ASafeSpawnLink>(Child);
				if (Link != NULL)
				{
					out_LinkedRI = Link;
					break;
				}
			}
		}

		return out_LinkedRI != NULL;
	}
};