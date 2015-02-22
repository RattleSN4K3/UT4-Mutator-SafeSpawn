#include "SafeSpawn.h"

//#include "Core.h"
//#include "Engine.h"
#include "ModuleManager.h"
#include "ModuleInterface.h"

class FSafeSpawnPlugin : public IModuleInterface
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE( FSafeSpawnPlugin, SafeSpawn )

void FSafeSpawnPlugin::StartupModule()
{
	
}

void FSafeSpawnPlugin::ShutdownModule()
{
	
}
