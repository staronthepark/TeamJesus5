#include "../Player/PlayerStatComponent.h"

UPlayerStatComponent::UPlayerStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<APlayerCharacter>(GetOwner());

	StrengthStatList[0].Func = [&]()
	{
		Owner->PlayerDataStruct.BaseDamage += Owner->PlayerDataStruct.BaseDamage * StrengthStatList[0].Value;
		Owner->PlayerDataStruct.StrengthIndex++;
	};
	StrengthStatList[1].Func = [&]()
	{
		Owner->PlayerDataStruct.BaseDamage += Owner->PlayerDataStruct.BaseDamage * StrengthStatList[1].Value;
		Owner->PlayerDataStruct.StrengthIndex++;
	};
	StrengthStatList[2].Func = [&]()
	{
		Owner->PlayerDataStruct.BaseDamage += Owner->PlayerDataStruct.BaseDamage * StrengthStatList[2].Value;
		Owner->PlayerDataStruct.StrengthIndex++;
	};
	StrengthStatList[3].Func = [&]()
	{
		Owner->PlayerDataStruct.BaseDamage += Owner->PlayerDataStruct.BaseDamage * StrengthStatList[3].Value;
		Owner->PlayerDataStruct.StrengthIndex++;
	};
	StrengthStatList[4].Func = [&]()
	{
		Owner->PlayerDataStruct.BaseDamage += Owner->PlayerDataStruct.BaseDamage * StrengthStatList[4].Value;
		Owner->PlayerDataStruct.StrengthIndex++;
	};

	StaminaStatList[0].Func = [&]()
	{
		Owner->PlayerDataStruct.MaxStamina += Owner->PlayerDataStruct.MaxStamina * StaminaStatList[0].Value;
		Owner->PlayerDataStruct.StaminaIndex++;
	};
	StaminaStatList[1].Func = [&]()
	{
		Owner->PlayerDataStruct.MaxStamina += Owner->PlayerDataStruct.MaxStamina * StaminaStatList[1].Value;
		Owner->PlayerDataStruct.StaminaIndex++;
	};
	StaminaStatList[2].Func = [&]()
	{
		Owner->PlayerDataStruct.MaxStamina += Owner->PlayerDataStruct.MaxStamina * StaminaStatList[2].Value;
		Owner->PlayerDataStruct.StaminaIndex++;
	};
	StaminaStatList[3].Func = [&]()
	{
		Owner->PlayerUseStaminaMap[ActionType::DODGE] -= Owner->PlayerUseStaminaMap[ActionType::DODGE] * StaminaStatList[3].Value;
		Owner->PlayerUseStaminaMap[ActionType::ATTACK] -= Owner->PlayerUseStaminaMap[ActionType::ATTACK] * StaminaStatList[3].Value;
		Owner->PlayerUseStaminaMap[ActionType::POWERATTACK] -= Owner->PlayerUseStaminaMap[ActionType::POWERATTACK] * StaminaStatList[3].Value;
		Owner->PlayerUseStaminaMap[ActionType::PARRING] -= Owner->PlayerUseStaminaMap[ActionType::PARRING] * StaminaStatList[3].Value;
		Owner->PlayerUseStaminaMap[ActionType::SHIELD] -= Owner->PlayerUseStaminaMap[ActionType::SHIELD] * StaminaStatList[3].Value;
		Owner->PlayerDataStruct.StaminaIndex++;
	};
	StaminaStatList[4].Func = [&]()
	{
		Owner->PlayerUseStaminaMap[ActionType::DODGE] -= Owner->PlayerUseStaminaMap[ActionType::DODGE] * StaminaStatList[4].Value;
		Owner->PlayerUseStaminaMap[ActionType::ATTACK] -= Owner->PlayerUseStaminaMap[ActionType::ATTACK] * StaminaStatList[4].Value;
		Owner->PlayerUseStaminaMap[ActionType::POWERATTACK] -= Owner->PlayerUseStaminaMap[ActionType::POWERATTACK] * StaminaStatList[4].Value;
		Owner->PlayerUseStaminaMap[ActionType::PARRING] -= Owner->PlayerUseStaminaMap[ActionType::PARRING] * StaminaStatList[4].Value;
		Owner->PlayerUseStaminaMap[ActionType::SHIELD] -= Owner->PlayerUseStaminaMap[ActionType::SHIELD] * StaminaStatList[4].Value;
		Owner->PlayerDataStruct.StaminaIndex++;
	};

	HpStatList[0].Func = [&]()
	{
		Owner->PlayerDataStruct.CharacterMaxHp += Owner->PlayerDataStruct.CharacterMaxHp * HpStatList[0].Value;
		Owner->PlayerDataStruct.HPIndex++;
	};
	HpStatList[1].Func = [&]()
	{
		Owner->PlayerDataStruct.CharacterMaxHp += Owner->PlayerDataStruct.CharacterMaxHp * HpStatList[1].Value;
		Owner->PlayerDataStruct.HPIndex++;
	};
	HpStatList[2].Func = [&]()
	{
		Owner->PlayerDataStruct.CharacterMaxHp += Owner->PlayerDataStruct.CharacterMaxHp * HpStatList[2].Value;
		Owner->PlayerDataStruct.HPIndex++;
	};
	HpStatList[3].Func = [&]()
	{
		Owner->PlayerDataStruct.PlayerHealValue += Owner->PlayerDataStruct.PlayerHealValue * HpStatList[3].Value;
		Owner->PlayerDataStruct.HPIndex++;
	};
	HpStatList[4].Func = [&]()
	{
		Owner->PlayerDataStruct.MaxHealCount += 1;
		Owner->PlayerDataStruct.HPIndex++;
	};

	ShieldStatList[0].Func = [&]()
	{
		Owner->PlayerDataStruct.ShieldIndex++;
	};
	ShieldStatList[1].Func = [&]()
	{
		Owner->PlayerDataStruct.ShieldIndex++;
	};
	ShieldStatList[2].Func = [&]()
	{
		Owner->PlayerDataStruct.ShieldIndex++;
	};
	ShieldStatList[3].Func = [&]()
	{
		Owner->PlayerDataStruct.ShieldIndex++;
	};
	ShieldStatList[4].Func = [&]()
	{
		Owner->PlayerDataStruct.ShieldIndex++;
	};
}