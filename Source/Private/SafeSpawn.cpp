#include "SafeSpawn.h"

//**********************************************************************************
// Private static functions
//**********************************************************************************

UMaterialInterface* USafeSpawn::GetGhostMaterial()
{
	return NULL;
}

USoundBase* USafeSpawn::GetGhostAmbientSound()
{
	return NULL;
}

//**********************************************************************************
// Static functions
//**********************************************************************************

void USafeSpawn::SetPPEffectsFor(APlayerController* PC, bool bAdd, bool& bOriginalOverridePostProcessSettings, FPostProcessSettings& OriginalPostProcessSettingsOverride, bool& PP_Scene_Changed)
{

}

void USafeSpawn::SetGhostFor(AUTCharacter* P, bool bTurnOn, const FGhostCollisionInfo& bOriginals /*= FGhostCollisionInfo()*/, bool bUseDefault/* = false*/)
{

}

void USafeSpawn::SetGhostEffectFor(AUTCharacter* P, bool bTurnOn)
{

}

void USafeSpawn::SetSkinEx(AUTCharacter* P, UMaterialInterface* NewMaterial)
{

}

void USafeSpawn::SetGhostSoundFor(AUTCharacter* P, bool bTurnOn)
{

}

void USafeSpawn::SetCrosshairFor(bool bRemoveCross, TArray<FCrosshairRestoreInfo>& CrosshairRestore)
{

}

void USafeSpawn::SetThirdPersonFor(APlayerController* UTPC, bool bEnable, /*const*/ bool& bOriginalBehindView /*= false*/)
{

}

void USafeSpawn::CheckSpawnKill(AUTCharacter* Other)
{

}

void USafeSpawn::PlayFireBlockedWarningFor(APlayerController* PlayerOwner)
{
	
}

bool USafeSpawn::ShouldIgnoreInputForNow(float StoredTime, float CurrentTime)
{
	//if (GetDefault()->IgnoreInputThreshold < 0)
	//{
	//	return true;
	//}

	//return (StoredTime + GetDefault()->IgnoreInputThreshold > CurrentTime);

	return true;
}
