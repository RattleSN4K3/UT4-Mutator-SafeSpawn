#pragma once

#include "Core.h"
#include "Engine.h"
//#include "UnrealTournament.h"
//#include "UTTeamInterface.h"
//#include "UTBasePlayerController.h"
//#include "UTPlayerController.h"

//**********************************************************************************
// Delegates
//**********************************************************************************

class ASafeSpawnRepInfo;
DECLARE_DELEGATE_TwoParams(FUnProtectFireDelegate, APlayerController*, ASafeSpawnRepInfo*);
//DECLARE_DELEGATE_TwoParams(FUnProtectFireDelegate, class APlayerController*, class ASafeSpawnRepInfo*);

DECLARE_DELEGATE_OneParam(FUnProtectPickupDelegate, ACharacter*);

class USafeSpawn : public UObject
{
};