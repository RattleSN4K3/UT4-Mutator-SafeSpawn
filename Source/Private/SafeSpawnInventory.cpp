#include "SafeSpawn.h"
#include "Engine/Canvas.h"
#include "UTHUDWidget.h"
#include "SafeSpawnInventory.h"

ASafeSpawnInventory::ASafeSpawnInventory(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	bCallOwnerEvent = 1;
	bAlwaysDropOnDeath = false;
}

//**********************************************************************************
// Inherited funtions
//**********************************************************************************

void ASafeSpawnInventory::Removed()
{
	Super::Removed();

}

void ASafeSpawnInventory::DrawInventoryHUD_Implementation(UUTHUDWidget* Widget, FVector2D Pos, FVector2D Size)
{
	// TODO: Implement basic canvas drawing
	UCanvas* Canvas = Widget->GetCanvas();
	Canvas->SetDrawColor(0, 255, 0);
	Canvas->DrawText(GEngine->GetSmallFont(), FText::FromString(TEXT("Test Draw")), 4.0f, 200.0f);
}

//**********************************************************************************
// Public funtions
//**********************************************************************************

void ASafeSpawnInventory::SetupInventory(AUTCharacter* Other, USoundBase* InWarningSound, float InGhostProtectionTime, FUnProtectPickupDelegate UnProtectDelegate)
{

}

// Allows inventory items to decide if a widget should be allowed to render them.
bool ASafeSpawnInventory::HUDShouldRender(UUTHUDWidget* TargetWidget)
{
	return true; // (TargetWidget && Cast<UUTHUDWidget_Powerups>(TargetWidget));
}