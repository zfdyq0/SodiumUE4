// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "SodiumUE4PrivatePCH.h"
#include "Core.h"
#include "ModuleManager.h"
#include "IPluginManager.h"

#if PLATFORM_WINDOWS
    #include "../../ThirdParty/SodiumUE4Library/SodiumUE4.h"
#endif // PLATFORM_WINDOWS

#define LOCTEXT_NAMESPACE "FSodiumUE4Module"

void FSodiumUE4Module::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("SodiumUE4")->GetBaseDir();

	// Add on the relative location of the third party dll and load it
	FString LibraryPath;
#if PLATFORM_WINDOWS
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/SodiumUE4Library/x64/Debug/libsodiumUE4.dll"));

	libsodiumUE4Handle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;

	if (libsodiumUE4Handle)
	{
		// load sodium
		int loadMsg = LoadSodium();
		if (loadMsg < 0) {
			UE_LOG(LogInit, Warning, TEXT("Sodium library failed to load."));
		} else {
			UE_LOG(LogInit, Log, TEXT("Sodium dll library successfully loaded."));

			bool sodiumTestPassed = SodiumTest();
			if(sodiumTestPassed){
				UE_LOG(LogInit, Log, TEXT("Sodium dll library test pass status: passed."));
			}else{
				UE_LOG(LogInit, Warning, TEXT("Sodium dll library test pass status: failed."));
			}
			
			// check if module is loaded into UE
			if (FSodiumUE4Module::IsAvailable()) {
				UE_LOG(LogInit, Log, TEXT("Sodium plugin successfully loaded."));
			} else {
				UE_LOG(LogInit, Warning, TEXT("Sodium plugin failed to load."));
			}
		}
	}
	else
#endif // PLATFORM_WINDOWS
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("ThirdPartyLibraryError", "Failed to load sodium-ue4."));
	}
}

void FSodiumUE4Module::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// Free the dll handle
	FPlatformProcess::FreeDllHandle(libsodiumUE4Handle);
	libsodiumUE4Handle = nullptr;
}

void FSodiumUE4Module::GenerateKeyPair(unsigned char *pk, unsigned char *sk) {
	int msg = SodiumGenerateKeyPair(pk, sk);
}

bool FSodiumUE4Module::Test() {
	return SodiumTest();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSodiumUE4Module, SodiumUE4)