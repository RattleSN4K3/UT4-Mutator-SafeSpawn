#include "SafeSpawn.h"

USafeSpawn::USafeSpawn(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

//**********************************************************************************
// Private static functions
//**********************************************************************************

static UMaterialInterface GetGhostMaterial()
{

}

static USoundBase GetGhostAmbientSound()
{

}

//**********************************************************************************
// Static functions
//**********************************************************************************

//static void SetPPEffectsFor(APlayerController PC, bool bAdd, uint8& bOriginalOverridePostProcessSettings, PostProcessSettings& OriginalPostProcessSettingsOverride, uint8& PP_Scene_Changed);

//static void SetGhostFor(AUTCharacter* P, bool bTurnOn)
//{
//	FGhostCollisionInfo bOriginals;
//	USafeSpawn::SetGhostFor(P, bTurnOn, bOriginals, false);
////}
//static void SetGhostFor(AUTCharacter* P, bool bTurnOn, FGhostCollisionInfo& bOriginals, bool bUseDefault) {}
//static void SetGhostEffectFor(AUTCharacter* P, bool bTurnOn) {}
//static void SetSkinEx(AUTCharacter* P, UMaterialInterface NewMaterial) {}
//static void SetGhostSoundFor(AUTCharacter* P, bool bTurnOn) {}
//static void SetCrosshairFor(bool bRemoveCross, TArray<FCrosshairRestoreInfo>& CrosshairRestore) {}
//
////static void SetThirdPersonFor(APlayerController* UTPC, bool bEnable)
////{
////	uint8 bOriginalBehindView;
////	USafeSpawn::SetThirdPersonFor(UTPC, bEnable, bOriginalBehindView);
////}
//static void SetThirdPersonFor(APlayerController* UTPC, bool bEnable, uint8& bOriginalBehindView) {}
//
//static void CheckSpawnKill(AUTCharacter* Other) {}
//static void PlayFireBlockedWarningFor(APlayerController* PlayerOwner) {}
