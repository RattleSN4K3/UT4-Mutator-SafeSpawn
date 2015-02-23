#pragma once

#include "Core.h"
#include "Engine.h"
#include "UTCharacter.h"
//#include "UTPlayerController.h"

#include "SafeSpawn.h"

#include "SafeSpawnRepInfo.generated.h"

UCLASS(Blueprintable)
class ASafeSpawnRepInfo : public AInfo
{
	GENERATED_UCLASS_BODY()


protected:

	//'''''''''''''''''''''''''
	// Workflow variables
	//'''''''''''''''''''''''''

	FUnProtectFireDelegate UnProtectCallback;
	FTimerHandle TimerHandle_StopBlockingFire;

	UPROPERTY()
	bool PP_Scene_Changed;

	UPROPERTY()
	bool bFireCalled;


	// Stored variables

	//UPROPERTY()
	//ASafeSpawnInteraction* ClientInteraction;
	UPROPERTY()
	AUTCharacter* OldPawn;

	UPROPERTY()
	bool bOriginalBehindView;

	UPROPERTY()
	bool bOriginalCollideActors;
	UPROPERTY()
	bool bOriginalBlockActors;
	UPROPERTY()
	bool bOriginalPushesRigidBodies;
	UPROPERTY()
	bool bOriginalIgnoreForces;

	UPROPERTY()
	bool bOriginalOverridePostProcessSettings;
	UPROPERTY()
	FPostProcessSettings OriginalPostProcessSettingsOverride;

	UPROPERTY()
	float PawnCounterTime;

	//'''''''''''''''''''''''''
	// Server variables
	//'''''''''''''''''''''''''

	UPROPERTY()
	bool bFireReceived;

	//'''''''''''''''''''''''''
	// Replication variables
	//'''''''''''''''''''''''''

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_PlayerOwner)
	APlayerController* PlayerOwner;
	UFUNCTION()
	virtual void OnRep_PlayerOwner();

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_PawnCounter)
	AUTCharacter* PawnCounter;
	UFUNCTION()
	virtual void OnRep_PawnCounter();

	UPROPERTY(Replicated)
	float InitialFireDelay;


	//'''''''''''''''''''''''''
	// Init funtions
	//'''''''''''''''''''''''''

public:
	void InitialSetup(APlayerController* PC, FUnProtectFireDelegate UnProtectDelegate, float InInitialFireDelay);
	
protected:
	void SetupInteraction(APlayerController* PC, bool bAdd);


	//'''''''''''''''''''''''''
	// Delegate Callbacks
	//'''''''''''''''''''''''''

protected:
	void OnFireInput();


	//'''''''''''''''''''''''''
	// Client funtions
	//'''''''''''''''''''''''''

	UFUNCTION(Client, Reliable)
	void ClientSetActive();

	UFUNCTION(Client, Reliable)
	void ClientUnblockInput();


	//'''''''''''''''''''''''''
	// Server funtions
	//'''''''''''''''''''''''''

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFired();


	//'''''''''''''''''''''''''
	// Public functions
	//'''''''''''''''''''''''''

public:

	/** CALLED SERVERSIDED ONLY */
	void NotifyRespawned(AUTCharacter* Other);

	/** CALLED SERVERSIDED ONLY */
	void NotifyActive();

	//'''''''''''''''''''''''''
	// Timed funtions
	//'''''''''''''''''''''''''

protected:

	void StopBlockingFire();


	//'''''''''''''''''''''''''
	// Private functions
	//'''''''''''''''''''''''''

protected:

	void UpdateGhostFor(AUTCharacter* P, bool bEnable);

	//'''''''''''''''''''''''''
	// Ghost protection funtions
	//'''''''''''''''''''''''''

	/** applies and removes any post processing effects while holding this item */
	void SetPPEffects(APlayerController* PC, bool bAdd);

	void SetGhost(AUTCharacter* P, bool bTurnOn);
	void SetGhostEffect(AUTCharacter* P, bool bTurnOn);
	void SetThirdPerson(APlayerController* PC, bool bEnable);

	//'''''''''''''''''''''''''
	// Helper
	//'''''''''''''''''''''''''

	// TODO: Should bLocallyOwned be inline?
	// For use with listen servers
	inline bool bLocallyOwned()
	{
		AActor* Owner = GetOwner();
		if (Owner == NULL)
			return false;

		if (GWorld->GetNetMode() != NM_DedicatedServer && Owner != NULL && Cast<ULocalPlayer>(Cast<APlayerController>(Owner)->Player) != NULL)
			return true;

		return false;
	}
};