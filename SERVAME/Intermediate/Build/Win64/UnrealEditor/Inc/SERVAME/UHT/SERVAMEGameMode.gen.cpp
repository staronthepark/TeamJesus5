// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "SERVAME/SERVAMEGameMode.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeSERVAMEGameMode() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	SERVAME_API UClass* Z_Construct_UClass_ASERVAMEGameMode();
	SERVAME_API UClass* Z_Construct_UClass_ASERVAMEGameMode_NoRegister();
	UPackage* Z_Construct_UPackage__Script_SERVAME();
// End Cross Module References
	void ASERVAMEGameMode::StaticRegisterNativesASERVAMEGameMode()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ASERVAMEGameMode);
	UClass* Z_Construct_UClass_ASERVAMEGameMode_NoRegister()
	{
		return ASERVAMEGameMode::StaticClass();
	}
	struct Z_Construct_UClass_ASERVAMEGameMode_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ASERVAMEGameMode_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_SERVAME,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ASERVAMEGameMode_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "SERVAMEGameMode.h" },
		{ "ModuleRelativePath", "SERVAMEGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ASERVAMEGameMode_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ASERVAMEGameMode>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_ASERVAMEGameMode_Statics::ClassParams = {
		&ASERVAMEGameMode::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x008802ACu,
		METADATA_PARAMS(Z_Construct_UClass_ASERVAMEGameMode_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ASERVAMEGameMode_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ASERVAMEGameMode()
	{
		if (!Z_Registration_Info_UClass_ASERVAMEGameMode.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ASERVAMEGameMode.OuterSingleton, Z_Construct_UClass_ASERVAMEGameMode_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_ASERVAMEGameMode.OuterSingleton;
	}
	template<> SERVAME_API UClass* StaticClass<ASERVAMEGameMode>()
	{
		return ASERVAMEGameMode::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(ASERVAMEGameMode);
	ASERVAMEGameMode::~ASERVAMEGameMode() {}
	struct Z_CompiledInDeferFile_FID_SERVAME_Source_SERVAME_SERVAMEGameMode_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_SERVAME_Source_SERVAME_SERVAMEGameMode_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_ASERVAMEGameMode, ASERVAMEGameMode::StaticClass, TEXT("ASERVAMEGameMode"), &Z_Registration_Info_UClass_ASERVAMEGameMode, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ASERVAMEGameMode), 1793825444U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_SERVAME_Source_SERVAME_SERVAMEGameMode_h_1256547422(TEXT("/Script/SERVAME"),
		Z_CompiledInDeferFile_FID_SERVAME_Source_SERVAME_SERVAMEGameMode_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_SERVAME_Source_SERVAME_SERVAMEGameMode_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
