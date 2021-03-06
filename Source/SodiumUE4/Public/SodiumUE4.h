#pragma once

#include "ModuleManager.h"

class FSodiumUE4Module : public IModuleInterface
{
public:

	////////////////////////////
	// IModuleInterface implementation
	////////////////////////////

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void GenerateKeyPair(TArray<uint8>& publicKey, TArray<uint8>& secretKey);

	int GetPublicKeyBytes();
	int GetSecretKeyBytes();
	int GetBoxSealBytes();
	int GetNonceBytes();
	int GetMacBytes();

	void RandomBytes(unsigned char* bytes, size_t len);

	int Encrypt(TArray<uint8>& encrypted, TArray<uint8>& data, TArray<uint8>& publicKey);
	int Decrypt(TArray<uint8>& decrypted, TArray<uint8>& encrypted, TArray<uint8>& publicKey, TArray<uint8>& privateKey);

	int EncryptAuthenticated(TArray<uint8>& encrypted, TArray<uint8>& data, TArray<uint8>& nonce, TArray<uint8>& publicKey, TArray<uint8>& privateKey);
	int DecryptAuthenticated(TArray<uint8>& decrypted, TArray<uint8>& encrypted, TArray<uint8>& nonce, TArray<uint8>& publicKey, TArray<uint8>& privateKey);

	static inline FSodiumUE4Module& Get() {
		return FModuleManager::LoadModuleChecked<FSodiumUE4Module>("SodiumUE4"); // name should be the same as directory of the plugin in /Plugins
	}

	static inline bool IsAvailable() {
		return FModuleManager::Get().IsModuleLoaded("SodiumUE4");
	}

private:
	/** Handle to the test dll we will load */
	void* libsodiumUE4Handle;
};