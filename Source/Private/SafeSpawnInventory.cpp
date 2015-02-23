#include "SafeSpawn.h"
#include "UnrealNetwork.h"
#include "UTGameplayStatics.h"
#include "Engine/Canvas.h"
#include "UTHUDWidget.h"
#include "UTWeapon.h"
#include "UTWeap_Enforcer.h"
#include "SafeSpawnInventory.h"

ASafeSpawnInventory::ASafeSpawnInventory(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	bCallOwnerEvent = 1;
	bAlwaysDropOnDeath = false;
}

//**********************************************************************************
// Replication
//**********************************************************************************

void ASafeSpawnInventory::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASafeSpawnInventory, InvSetup, COND_InitialOnly);
}

void ASafeSpawnInventory::OnRep_InvSetup()
{
	if (InvSetup.InvOwner != NULL)
	{
		SetOwner(InvSetup.InvOwner);

		BarTimUsed = FMath::Min(InvSetup.GhostProtectionTime, BarTimeThreshold);

		if (WarningSound)
		{
			// play in next tick (mainly for listen server support)
			GetWorldTimerManager().SetTimer(TimerHandle_ConditionalPlayWarning, this, &ASafeSpawnInventory::ConditionalPlayWarning, 0.001f, false);
		}

		ClientSetup(InvSetup.InvOwner, false);
	}
}

//**********************************************************************************
// Inherited funtions
//**********************************************************************************

// TODO: Wait for engine fix as AUTTimedPowerup is not exported properly
///** called when TimeRemaining reaches zero */
//void ASafeSpawnInventory::TimeExpired_Implementation()
//{
//	if (Role == ROLE_Authority)
//	{
//		//// store callback locally as it will be de-referenced in the Destroyed method
//		//FUnProtectPickup UnProtectDelegate = UnProtectCallback;
//
//		//Super::TimeExpired();
//
//		//// notify main module
//		//if (UnProtectDelegate != NULL && UnProtectDelegate.IsBound())
//		//{
//		//	UnProtectDelegate.Execute(GetInstigator<AUTCharacter>());
//		//}
//	}
//}

void ASafeSpawnInventory::Destroyed()
{
	AUTCharacter* P(GetUTOwner());
	if (P == NULL) P = InvSetup.InvOwner;

	if (P != NULL)
	{
		if (Role < ROLE_Authority)
		{
			ClientSetup(P, true);
		}
		else
		{
			FUnProtectPickupDelegate UnProtectDelegate = NULL;

			// notify main module
			if (UnProtectCallback.IsBound())
			{
				UnProtectDelegate = UnProtectCallback;
				UnProtectDelegate.Execute(GetInstigator<AUTCharacter>());
			}

			// clear reference
			//UnProtectCallback.Unbind();
			UnProtectCallback = NULL;

			// notify server
			if (UnProtectDelegate.IsBound() /*&& TimeRemaining > 0*/)
			{
				UnProtectDelegate.Execute(GetInstigator<AUTCharacter>());
			}
		}
	}

	Super::Destroyed();
}

void ASafeSpawnInventory::GivenTo(AUTCharacter* NewOwner, bool bAutoActivate)
{
	Super::GivenTo(NewOwner, bAutoActivate);

	AUTCharacter* P = Cast<AUTCharacter>(NewOwner);
	if (P != NULL)
	{
		// we disabled the collision, we need to check whether
		// we are trying to	pick something up differently
		AddCollisionCheck(true);

		//USafeSpawn::SetGhostSoundFor(P, true);
	}
}

void ASafeSpawnInventory::Removed()
{
	Super::Removed();

	AUTCharacter* P(GetUTOwner());
	if (Role == ROLE_Authority && P != NULL)
	{
		// for listen server support
		ClientSetup(P, true);

		//USafeSpawn::SetGhostSoundFor(P, false);
	}
}

void ASafeSpawnInventory::OwnerEvent_Implementation(FName EventName)
{
	static FName NAME_Landed(TEXT("Landed"));
	static FName NAME_ChangedWeapon(TEXT("ChangedWeapon"));
	static FName NAME_FiredWeapon(TEXT("FiredWeapon"));
	
	if (Role == ROLE_Authority)
	{
		bool bBreak = false;

		if (EventName == NAME_Landed)
		{
			WaitForLandedTime = GetWorld()->GetTimeSeconds();
			bBreak = true;
		}
		else if (EventName == NAME_ChangedWeapon)
		{
			//if (UTInventoryManager(Instigator.InvManager) != none && UTInventoryManager(Instigator.InvManager).PreviousWeapon == none)
			//{
			//	// bugs for clients as the event gets called immediately
			//	// so we need to abort this call once for the weapon change on spawn (where the previous weapon is none)
			//	break;
			//}
			//else
			if (bWaitForLanded || WaitForLandedTime == GetWorld()->GetTimeSeconds())
			{
				bWaitForLanded = false;
				bBreak = true;
			}
		}
		
		if (EventName == NAME_FiredWeapon || bBreak == false) // fall through
		{
			//TimeExpired();
		}
	}
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
	FSetupInfo LocalSetup;

	//TimeRemaining = InGhostProtectionTime;
	UnProtectCallback = UnProtectDelegate;

	// replicate setup variables to client
	LocalSetup.GhostProtectionTime = InGhostProtectionTime;
	LocalSetup.WarningSound = InWarningSound;
	LocalSetup.InvOwner = Other;

	InvSetup = LocalSetup;
	if (GWorld->GetNetMode() != NM_DedicatedServer && bLocallyOwned())
	{
		OnRep_InvSetup();
	}
}

// Allows inventory items to decide if a widget should be allowed to render them.
bool ASafeSpawnInventory::HUDShouldRender(UUTHUDWidget* TargetWidget)
{
	return true; // (TargetWidget && Cast<UUTHUDWidget_Powerups>(TargetWidget));
}

//**********************************************************************************
// Private funtions
//**********************************************************************************

/** Serversided only */
void ASafeSpawnInventory::AddCollisionCheck(bool add)
{
	//local PrimitiveComponent OrgComp, Comp;

	//if (add)
	//{
	//	OrgComp = Owner.CollisionComponent;
	//	Comp = new(self)OrgComp.Class(OrgComp);
	//	AttachComponent(Comp);
	//	Comp.SetActorCollision(true, false);
	//	CollisionComponent = Comp;
	//	SetCollision(true, false, false);

	//	SetLocation(Instigator.Location);
	//	SetBase(Instigator);
	//}
	//else
	//{
	//	SetCollision(false, false, false);
	//	DetachComponent(CollisionComponent);
	//	CollisionComponent = none;

	//	SetBase(none);
	//}
}

/** adds or removes our bonus from the given pawn */
void ASafeSpawnInventory::ClientSetup(AUTCharacter* P, bool bRemove)
{
	if ((!bRemove || bAlreadySetup) && (Role < ROLE_Authority || GWorld->GetNetMode() != NM_DedicatedServer))
	{
		bAlreadySetup = true;
		if (bLocallyOwned())
		{
			SetCrosshair(P, !bRemove);
			//BlockWeapons(P, !bRemove);

			if (!bRemove)
			{
				FixWeapons(P);
			}
		}
	}
}

void ASafeSpawnInventory::ConditionalPlayWarning()
{
	if (bPlayWarningOnce)
	{
		PlayWarningSound();
		bPlayWarningOnce = false;
	}

	//if (TimeRemaining > 10.0)
	//{
	//	bPlayWarningOnce = true;
	//	SetTimer(TimeRemaining - 10.0, false, GetFuncName());
	//}
	//if (TimeRemaining > 5.0)
	//{
	//	bPlayWarningOnce = true;
	//	SetTimer(TimeRemaining - 5.0, false, GetFuncName());
	//}
	//else
	//{
	//	if (TimeRemaining < 2.0)
	//	{
	//		PlayWarningSound();
	//		SetTimer(0.65, false, GetFuncName());
	//	}
	//	else
	//	{
	//		bPlayWarningOnce = true;
	//		SetTimer(1.5, false, GetFuncName());
	//	}
	//}
}

void ASafeSpawnInventory::PlayWarningSound()
{
	AActor* Other = (GetOwner() == NULL ? Instigator : GetOwner());
	if (Other != NULL && InvSetup.WarningSound != NULL)
	{
		// TODO: check if played correctly
		UUTGameplayStatics::UTPlaySound(GetWorld(), InvSetup.WarningSound, this, SRT_None);
	}
}

void ASafeSpawnInventory::SetCrosshair(AUTCharacter* P, bool bRemoveCross)
{
	//USafeSpawn::SetCrosshairFor(bRemoveCross, CrosshairRestore);
}

void ASafeSpawnInventory::BlockWeapons(AUTCharacter* P, bool bBlock)
{
	int32 index;
	for (TInventoryIterator<AUTWeapon> It(P); It; ++It)
	{
		AUTWeapon* Weap = *It;
		if (bBlock)
		{
			index = WeaponRestore.Num();
			WeaponRestore.AddZeroed(1);
			WeaponRestore[index].Weap = Weap;
			WeaponRestore[index].FiringState = Weap->FiringState;
			//WeaponRestore[index].WeaponFireTypes = Weap.WeaponFireTypes;

			//for (i = 0; i<Weap.WeaponFireTypes.Length; i++)
			//{
			//	Weap.WeaponFireTypes[i] = EWFT_None;
			//}

			for (int32 i = 0; i<Weap->FiringState.Num(); i++)
			{
				Weap->FiringState[i] = NULL;
			}
		}
		else
		{
			// TODO: find a better suited replacement. UC: Array.Find('Key', VALUE);
			for (int32 i = 0; i<WeaponRestore.Num(); i++)
			{
				if (WeaponRestore[i].Weap == Weap)
				{
					Weap->FiringState = WeaponRestore[i].FiringState;
					//Weap.WeaponFireTypes = WeaponRestore[index].WeaponFireTypes;
					break;
				}
			}
		}
	}
}

void ASafeSpawnInventory::FixWeapons(AUTCharacter* P)
{
	if (P == NULL)
		return;

	//for (TInventoryIterator<AUTWeap_Enforcer> Enf(P); Enf; ++Enf)
	//{
	//	Enf->bLoaded = true;
	//}
}
