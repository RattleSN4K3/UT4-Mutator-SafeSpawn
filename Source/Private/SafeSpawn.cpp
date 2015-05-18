#include "SafeSpawn.h"
//#include "UTPlayerController.h"
#include "UTHUDWidget_WeaponCrosshair.h"

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

void USafeSpawn::SetCrosshairFor(APlayerController* PC, bool bRemoveCross, /*const*/ bool& bOriginalCrosshairHidden/* = false*/)
{
	// if disabled, abort
	if (GetDefault<USafeSpawn>()->HideCrosshairTemporarely || PC == NULL || PC->MyHUD == NULL)
		return;

	AUTHUD* Hud = Cast<AUTHUD>(PC->MyHUD);
	if (Hud == NULL)
		return;

	UUTHUDWidget_WeaponCrosshair* Crosshair = Cast<UUTHUDWidget_WeaponCrosshair>(Hud->FindHudWidgetByClass(UUTHUDWidget_WeaponCrosshair::StaticClass()));
	if (Crosshair == NULL)
		return;

	Crosshair->SetHidden(bRemoveCross);
}

void USafeSpawn::SetThirdPersonFor(APlayerController* PC, bool bEnable, /*const*/ bool& bOriginalBehindView /*= false*/)
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
