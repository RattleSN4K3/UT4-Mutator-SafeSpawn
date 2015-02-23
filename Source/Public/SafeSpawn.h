#pragma once

#include "Core.h"
#include "Engine.h"

#include "UTCharacter.h"

//#include "MaterialInterface.h"

//#include "UnrealTournament.h"
//#include "UTTeamInterface.h"
//#include "UTBasePlayerController.h"
//#include "UTPlayerController.h"

#include "SafeSpawn.generated.h"

//**********************************************************************************
// Structs
//**********************************************************************************

USTRUCT()
struct FCrosshairRestoreInfo
{
	GENERATED_USTRUCT_BODY()

	//UPROPERTY()
	//AUTWeapon* Weap;

	//UPROPERTY()
	//UTexture2D* CrosshairImage;
};

USTRUCT()
struct FWeaponRestoreInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	AUTWeapon* Weap;

	/** Array of firing states defining available firemodes */
	UPROPERTY()
	TArray<class UUTWeaponStateFiring*> FiringState;

	/////** Defines the type of fire (see Enum above) for each mode */
	////var				Array<EWeaponFireType>		WeaponFireTypes;
};

USTRUCT()
struct FGhostCollisionInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	bool bSet;

	UPROPERTY()
	bool bOriginalCollideActors;
	UPROPERTY()
	bool bOriginalBlockActors;
	UPROPERTY()
	bool bOriginalPushesRigidBodies;
	UPROPERTY()
	bool bOriginalIgnoreForces;
};

//**********************************************************************************
// Delegates
//**********************************************************************************

class ASafeSpawnRepInfo;
DECLARE_DELEGATE_TwoParams(FUnProtectFireDelegate, APlayerController*, ASafeSpawnRepInfo*);
//DECLARE_DELEGATE_TwoParams(FUnProtectFireDelegate, class APlayerController*, class ASafeSpawnRepInfo*);

DECLARE_DELEGATE_OneParam(FUnProtectPickupDelegate, ACharacter*);

UCLASS()
class USafeSpawn : public UObject
{
	GENERATED_UCLASS_BODY()

	//static bool bDefaultVal = false;

	//'''''''''''''''''''''''''
	// Private static functions
	//'''''''''''''''''''''''''

	static UMaterialInterface GetGhostMaterial();
	static USoundBase GetGhostAmbientSound();

	//'''''''''''''''''''''''''
	// Static functions
	//'''''''''''''''''''''''''

	static void SetPPEffectsFor(APlayerController PC, bool bAdd, bool& bOriginalOverridePostProcessSettings, FPostProcessSettings& OriginalPostProcessSettingsOverride, bool& PP_Scene_Changed);

	// TODO: Add overload static method 
	//static void SetGhostFor(AUTCharacter* P, bool bTurnOn);
	static void SetGhostFor(AUTCharacter* P, bool bTurnOn, const FGhostCollisionInfo& bOriginals = FGhostCollisionInfo(), bool bUseDefault = false);
	static void SetGhostEffectFor(AUTCharacter* P, bool bTurnOn);
	static void SetSkinEx(AUTCharacter* P, UMaterialInterface NewMaterial);
	static void SetGhostSoundFor(AUTCharacter* P, bool bTurnOn);
	static void SetCrosshairFor(bool bRemoveCross, TArray<FCrosshairRestoreInfo>& CrosshairRestore);
	
	static void SetThirdPersonFor(APlayerController* UTPC, bool bEnable, const bool& bOriginalBehindView = false);

	static void CheckSpawnKill(AUTCharacter* Other);
	static void PlayFireBlockedWarningFor(APlayerController* PlayerOwner);

	static bool ShouldIgnoreInputForNow(float StoredTime, float CurrentTime)
	{
		/*if (GetDefault()->IgnoreInputThreshold < 0)
		{
			return true;
		}

		return (StoredTime + GetDefault()->IgnoreInputThreshold > CurrentTime);*/
		return true;
	}

	inline FLinearColor NormalizeColor(FLinearColor LC)
	{

		//local vector v;
		//v.X = LC.R;
		//v.Y = LC.G;
		//v.Z = LC.B;

		//v = Normal(v);
		//return MakeLinearColor(v.X, v.Y, v.Z, 1.0);
	}

	inline FLinearColor BoostColor(FLinearColor LC, float strength)
	{
		//local vector v;
		//v.X = LC.R**4;
		//v.Y = LC.G**4;
		//v.Z = LC.B**4;

		//v = Normal(v);
		//v *= strength;
		//return MakeLinearColor(v.X, v.Y, v.Z, 1.0);
	}
};