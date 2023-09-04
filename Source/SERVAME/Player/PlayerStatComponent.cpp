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
		Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK1].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK1].Damage * StrengthStatList[0].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK2].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK2].Damage * StrengthStatList[0].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK3].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK3].Damage * StrengthStatList[0].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK4].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK4].Damage * StrengthStatList[0].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::POWERATTACK1].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::POWERATTACK1].Damage * StrengthStatList[0].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::POWERATTACK2].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::POWERATTACK2].Damage * StrengthStatList[0].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::POWERATTACK3].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::POWERATTACK3].Damage * StrengthStatList[0].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::RUNPOWERATTACK].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::RUNPOWERATTACK].Damage * StrengthStatList[0].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::DODGEATTACK].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::DODGEATTACK].Damage * StrengthStatList[0].Value;
		Owner->PlayerDataStruct.PlayerExecutionFirstDamage += Owner->PlayerDataStruct.PlayerExecutionFirstDamage * StrengthStatList[0].Value;
		Owner->PlayerDataStruct.PlayerExecutionFirstDamage += Owner->PlayerDataStruct.PlayerExecutionSecondDamage * StrengthStatList[0].Value;
	};
	StrengthStatList[1].Func = [&]()
	{
		Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK1].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK1].Damage * StrengthStatList[1].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK2].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK2].Damage * StrengthStatList[1].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK3].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK3].Damage * StrengthStatList[1].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK4].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK4].Damage * StrengthStatList[1].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::POWERATTACK1].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::POWERATTACK1].Damage * StrengthStatList[1].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::POWERATTACK2].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::POWERATTACK2].Damage * StrengthStatList[1].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::POWERATTACK3].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::POWERATTACK3].Damage * StrengthStatList[1].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::RUNPOWERATTACK].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::RUNPOWERATTACK].Damage * StrengthStatList[1].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::DODGEATTACK].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::DODGEATTACK].Damage * StrengthStatList[1].Value;
		Owner->PlayerDataStruct.PlayerExecutionFirstDamage += Owner->PlayerDataStruct.PlayerExecutionFirstDamage * StrengthStatList[1].Value;
		Owner->PlayerDataStruct.PlayerExecutionFirstDamage += Owner->PlayerDataStruct.PlayerExecutionSecondDamage * StrengthStatList[1].Value;
	};
	StrengthStatList[2].Func = [&]()
	{
		Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK1].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK1].Damage * StrengthStatList[2].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK2].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK2].Damage * StrengthStatList[2].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK3].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK3].Damage * StrengthStatList[2].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK4].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK4].Damage * StrengthStatList[2].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::POWERATTACK1].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::POWERATTACK1].Damage * StrengthStatList[2].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::POWERATTACK2].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::POWERATTACK2].Damage * StrengthStatList[2].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::POWERATTACK3].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::POWERATTACK3].Damage * StrengthStatList[2].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::RUNPOWERATTACK].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::RUNPOWERATTACK].Damage * StrengthStatList[2].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::DODGEATTACK].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::DODGEATTACK].Damage * StrengthStatList[2].Value;
		Owner->PlayerDataStruct.PlayerExecutionFirstDamage += Owner->PlayerDataStruct.PlayerExecutionFirstDamage * StrengthStatList[2].Value;
		Owner->PlayerDataStruct.PlayerExecutionFirstDamage += Owner->PlayerDataStruct.PlayerExecutionSecondDamage * StrengthStatList[2].Value;
	};
	StrengthStatList[3].Func = [&]()
	{
		Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK1].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK1].Damage * StrengthStatList[3].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK2].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK2].Damage * StrengthStatList[3].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK3].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK3].Damage * StrengthStatList[3].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK4].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK4].Damage * StrengthStatList[3].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::POWERATTACK1].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::POWERATTACK1].Damage * StrengthStatList[3].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::POWERATTACK2].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::POWERATTACK2].Damage * StrengthStatList[3].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::POWERATTACK3].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::POWERATTACK3].Damage * StrengthStatList[3].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::RUNPOWERATTACK].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::RUNPOWERATTACK].Damage * StrengthStatList[3].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::DODGEATTACK].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::DODGEATTACK].Damage * StrengthStatList[3].Value;
		Owner->PlayerDataStruct.PlayerExecutionFirstDamage += Owner->PlayerDataStruct.PlayerExecutionFirstDamage * StrengthStatList[3].Value;
		Owner->PlayerDataStruct.PlayerExecutionFirstDamage += Owner->PlayerDataStruct.PlayerExecutionSecondDamage * StrengthStatList[3].Value;
	};
	StrengthStatList[4].Func = [&]()
	{
		Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK1].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK1].Damage * StrengthStatList[4].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK2].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK2].Damage * StrengthStatList[4].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK3].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK3].Damage * StrengthStatList[4].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK4].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::ATTACK4].Damage * StrengthStatList[4].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::POWERATTACK1].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::POWERATTACK1].Damage * StrengthStatList[4].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::POWERATTACK2].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::POWERATTACK2].Damage * StrengthStatList[4].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::POWERATTACK3].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::POWERATTACK3].Damage * StrengthStatList[4].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::RUNPOWERATTACK].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::RUNPOWERATTACK].Damage * StrengthStatList[4].Value;
		Owner->PlayerDataStruct.DamageList[AnimationType::DODGEATTACK].Damage += Owner->PlayerDataStruct.DamageList[AnimationType::DODGEATTACK].Damage * StrengthStatList[4].Value;
		Owner->PlayerDataStruct.PlayerExecutionFirstDamage += Owner->PlayerDataStruct.PlayerExecutionFirstDamage * StrengthStatList[4].Value;
		Owner->PlayerDataStruct.PlayerExecutionFirstDamage += Owner->PlayerDataStruct.PlayerExecutionSecondDamage * StrengthStatList[4].Value;
	};

	StaminaStatList[0].Func = [&]()
	{
		Owner->PlayerDataStruct.MaxStamina += Owner->PlayerDataStruct.MaxStamina * StaminaStatList[0].Value;
	};
	StaminaStatList[1].Func = [&]()
	{
		Owner->PlayerDataStruct.MaxStamina += Owner->PlayerDataStruct.MaxStamina * StaminaStatList[1].Value;
	};
	StaminaStatList[2].Func = [&]()
	{
		Owner->PlayerDataStruct.MaxStamina += Owner->PlayerDataStruct.MaxStamina * StaminaStatList[2].Value;
	};
	StaminaStatList[3].Func = [&]()
	{
		Owner->PlayerUseStaminaMap[ActionType::DODGE] -= Owner->PlayerUseStaminaMap[ActionType::DODGE] * StaminaStatList[3].Value;
		Owner->PlayerUseStaminaMap[ActionType::ATTACK] -= Owner->PlayerUseStaminaMap[ActionType::ATTACK] * StaminaStatList[3].Value;
		Owner->PlayerUseStaminaMap[ActionType::POWERATTACK] -= Owner->PlayerUseStaminaMap[ActionType::POWERATTACK] * StaminaStatList[3].Value;
		Owner->PlayerUseStaminaMap[ActionType::PARRING] -= Owner->PlayerUseStaminaMap[ActionType::PARRING] * StaminaStatList[3].Value;
		Owner->PlayerUseStaminaMap[ActionType::SHIELD] -= Owner->PlayerUseStaminaMap[ActionType::SHIELD] * StaminaStatList[3].Value;
	};
	StaminaStatList[4].Func = [&]()
	{
		Owner->PlayerUseStaminaMap[ActionType::DODGE] -= Owner->PlayerUseStaminaMap[ActionType::DODGE] * StaminaStatList[4].Value;
		Owner->PlayerUseStaminaMap[ActionType::ATTACK] -= Owner->PlayerUseStaminaMap[ActionType::ATTACK] * StaminaStatList[4].Value;
		Owner->PlayerUseStaminaMap[ActionType::POWERATTACK] -= Owner->PlayerUseStaminaMap[ActionType::POWERATTACK] * StaminaStatList[4].Value;
		Owner->PlayerUseStaminaMap[ActionType::PARRING] -= Owner->PlayerUseStaminaMap[ActionType::PARRING] * StaminaStatList[4].Value;
		Owner->PlayerUseStaminaMap[ActionType::SHIELD] -= Owner->PlayerUseStaminaMap[ActionType::SHIELD] * StaminaStatList[4].Value;
	};

	HpStatList[0].Func = [&]()
	{
		Owner->PlayerDataStruct.CharacterMaxHp += Owner->PlayerDataStruct.CharacterMaxHp * HpStatList[0].Value;
	};
	HpStatList[1].Func = [&]()
	{
		Owner->PlayerDataStruct.CharacterMaxHp += Owner->PlayerDataStruct.CharacterMaxHp * HpStatList[1].Value;
	};
	HpStatList[2].Func = [&]()
	{
		Owner->PlayerDataStruct.CharacterMaxHp += Owner->PlayerDataStruct.CharacterMaxHp * HpStatList[2].Value;
	};
	HpStatList[3].Func = [&]()
	{
		Owner->PlayerDataStruct.PlayerHealValue += Owner->PlayerDataStruct.PlayerHealValue * HpStatList[3].Value;
	};
	HpStatList[4].Func = [&]()
	{
		Owner->PlayerDataStruct.MaxHealCount += 1;
	};

	ShieldStatList[0].Func = [&]()
	{
		Owner->PlayerDataStruct.MaxShieldHP += Owner->PlayerDataStruct.MaxShieldHP * ShieldStatList[0].Value;
	};
	ShieldStatList[1].Func = [&]()
	{
		Owner->PlayerDataStruct.MaxShieldHP += Owner->PlayerDataStruct.MaxShieldHP * ShieldStatList[1].Value;
	};
	ShieldStatList[2].Func = [&]()
	{
		Owner->PlayerDataStruct.MaxShieldHP += Owner->PlayerDataStruct.MaxShieldHP * ShieldStatList[2].Value;
	};
	ShieldStatList[3].Func = [&]()
	{
		Owner->PlayerDataStruct.ShieldRecoverySoulCount -= 1;
	};
	ShieldStatList[4].Func = [&]()
	{
	};
}