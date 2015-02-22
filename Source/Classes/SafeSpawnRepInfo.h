#pragma once

#include "Core.h"
#include "Engine.h"
#include "UTCharacter.h"
//#include "UTPlayerController.h"

#include "SafeSpawnRepInfo.generated.h"

//class ASafeSpawnRepInfo;
//DECLARE_DELEGATE_TwoParams(FUnProtectFireDelegate, class APlayerController*, class ASafeSpawnRepInfo*);
DECLARE_DELEGATE(FUnProtectFireDelegate);

UCLASS(Blueprintable)
class ASafeSpawnRepInfo : public AInfo
{
	GENERATED_UCLASS_BODY()

	//'''''''''''''''''''''''''
	// Delegates
	//'''''''''''''''''''''''''

	FUnProtectFireDelegate UnProtectCallback;

	//'''''''''''''''''''''''''
	// Init funtions
	//'''''''''''''''''''''''''

	void InitialSetup(APlayerController* PC, FUnProtectFireDelegate UnProtectDelegate, float InInitialFireDelay);
	void SetupInteraction(APlayerController* PC, bool bAdd);


protected:

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
};