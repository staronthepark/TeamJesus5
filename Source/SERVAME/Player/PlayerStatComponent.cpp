#include "../Player/PlayerStatComponent.h"

UPlayerStatComponent::UPlayerStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<APlayerCharacter>(GetOwner());
	
	StrengthStatList[0].Func = [&]()-> bool
	{
		if (Owner->CanActivate(StrengthStatList[Owner->PlayerDataStruct.StrengthIndex].SoulCost))
		{
			Owner->PlayerDataStruct.BaseDamage += Owner->PlayerDataStruct.BaseDamage * StrengthStatList[Owner->PlayerDataStruct.StrengthIndex].Value;
			Owner->PlayerDataStruct.StrengthIndex++;
			return true;
		}
		return false;
	};
	StrengthStatList[1].Func = [&]()-> bool
	{
		if (Owner->CanActivate(StrengthStatList[Owner->PlayerDataStruct.StrengthIndex].SoulCost))
		{
			Owner->PlayerDataStruct.BaseDamage += Owner->PlayerDataStruct.BaseDamage * StrengthStatList[Owner->PlayerDataStruct.StrengthIndex].Value;
			Owner->PlayerDataStruct.StrengthIndex++;
			return true;
		}
		return false;
	};
	StrengthStatList[2].Func = [&]()-> bool
	{
		if (Owner->CanActivate(StrengthStatList[Owner->PlayerDataStruct.StrengthIndex].SoulCost))
		{
			Owner->PlayerDataStruct.BaseDamage += Owner->PlayerDataStruct.BaseDamage * StrengthStatList[Owner->PlayerDataStruct.StrengthIndex].Value;
			Owner->PlayerDataStruct.StrengthIndex++;
			return true;
		}
		return false;
	};
	StrengthStatList[3].Func = [&]()-> bool
	{
		if (Owner->CanActivate(StrengthStatList[Owner->PlayerDataStruct.StrengthIndex].SoulCost))
		{
			Owner->PlayerDataStruct.BaseDamage += Owner->PlayerDataStruct.BaseDamage * StrengthStatList[Owner->PlayerDataStruct.StrengthIndex].Value;
			Owner->PlayerDataStruct.StrengthIndex++;
			return true;
		}
		return false;
	};
	StrengthStatList[4].Func = [&]()-> bool
	{
		if (Owner->CanActivate(StrengthStatList[Owner->PlayerDataStruct.StrengthIndex].SoulCost))
		{
			Owner->PlayerDataStruct.BaseDamage += Owner->PlayerDataStruct.BaseDamage * StrengthStatList[Owner->PlayerDataStruct.StrengthIndex].Value;
			Owner->PlayerDataStruct.StrengthIndex++;
			return true;
		}
		return false;
	};

	StaminaStatList[0].Func = [&]()-> bool
	{
		if (Owner->CanActivate(StaminaStatList[Owner->PlayerDataStruct.StaminaIndex].SoulCost))
		{
			Owner->PlayerDataStruct.MaxStamina += Owner->PlayerDataStruct.MaxStamina * StaminaStatList[Owner->PlayerDataStruct.StaminaIndex].Value;
			Owner->PlayerDataStruct.StaminaIndex++;
			Owner->PlayerHUD->SetStamina(1.0f);
			return true;
		}
		return false;
	};
	StaminaStatList[1].Func = [&]()-> bool
	{
		if (Owner->CanActivate(StaminaStatList[Owner->PlayerDataStruct.StaminaIndex].SoulCost))
		{
			Owner->PlayerDataStruct.MaxStamina += Owner->PlayerDataStruct.MaxStamina * StaminaStatList[Owner->PlayerDataStruct.StaminaIndex].Value;
			Owner->PlayerDataStruct.StaminaIndex++;
			Owner->PlayerHUD->SetStamina(1.0f);
			return true;
		}
		return false;
	};
	StaminaStatList[2].Func = [&]()-> bool
	{
		if (Owner->CanActivate(StaminaStatList[Owner->PlayerDataStruct.StaminaIndex].SoulCost))
		{
			Owner->PlayerDataStruct.MaxStamina += Owner->PlayerDataStruct.MaxStamina * StaminaStatList[Owner->PlayerDataStruct.StaminaIndex].Value;
			Owner->PlayerDataStruct.StaminaIndex++;
			Owner->PlayerHUD->SetStamina(1.0f);
			return true;
		}
		return false;
	};
	StaminaStatList[3].Func = [&]()-> bool
	{
		if (Owner->CanActivate(StaminaStatList[Owner->PlayerDataStruct.StaminaIndex].SoulCost))
		{

			Owner->PlayerUseStaminaMap[ActionType::DODGE] -= Owner->PlayerUseStaminaMap[ActionType::DODGE] * StaminaStatList[Owner->PlayerDataStruct.StaminaIndex].Value;
			Owner->PlayerUseStaminaMap[ActionType::ATTACK] -= Owner->PlayerUseStaminaMap[ActionType::ATTACK] * StaminaStatList[Owner->PlayerDataStruct.StaminaIndex].Value;
			Owner->PlayerUseStaminaMap[ActionType::POWERATTACK] -= Owner->PlayerUseStaminaMap[ActionType::POWERATTACK] * StaminaStatList[Owner->PlayerDataStruct.StaminaIndex].Value;
			Owner->PlayerUseStaminaMap[ActionType::PARRING] -= Owner->PlayerUseStaminaMap[ActionType::PARRING] * StaminaStatList[Owner->PlayerDataStruct.StaminaIndex].Value;
			Owner->PlayerUseStaminaMap[ActionType::SHIELD] -= Owner->PlayerUseStaminaMap[ActionType::SHIELD] * StaminaStatList[Owner->PlayerDataStruct.StaminaIndex].Value;
			Owner->PlayerDataStruct.StaminaIndex++;
			return true;
		}
		return false;

	};
	StaminaStatList[4].Func = [&]()-> bool
	{
		if (Owner->CanActivate(StaminaStatList[Owner->PlayerDataStruct.StaminaIndex].SoulCost))
		{
			Owner->PlayerUseStaminaMap[ActionType::DODGE] -= Owner->PlayerUseStaminaMap[ActionType::DODGE] * StaminaStatList[Owner->PlayerDataStruct.StaminaIndex].Value;
			Owner->PlayerUseStaminaMap[ActionType::ATTACK] -= Owner->PlayerUseStaminaMap[ActionType::ATTACK] * StaminaStatList[Owner->PlayerDataStruct.StaminaIndex].Value;
			Owner->PlayerUseStaminaMap[ActionType::POWERATTACK] -= Owner->PlayerUseStaminaMap[ActionType::POWERATTACK] * StaminaStatList[Owner->PlayerDataStruct.StaminaIndex].Value;
			Owner->PlayerUseStaminaMap[ActionType::PARRING] -= Owner->PlayerUseStaminaMap[ActionType::PARRING] * StaminaStatList[Owner->PlayerDataStruct.StaminaIndex].Value;
			Owner->PlayerUseStaminaMap[ActionType::SHIELD] -= Owner->PlayerUseStaminaMap[ActionType::SHIELD] * StaminaStatList[Owner->PlayerDataStruct.StaminaIndex].Value;
			Owner->PlayerDataStruct.StaminaIndex++;
			return true;
		}
		return false;
	};

	HpStatList[0].Func = [&]()-> bool
	{
		if (Owner->CanActivate(HpStatList[Owner->PlayerDataStruct.HPIndex].SoulCost))
		{
			Owner->PlayerDataStruct.CharacterMaxHp += Owner->PlayerDataStruct.CharacterMaxHp * HpStatList[Owner->PlayerDataStruct.HPIndex].Value;
			Owner->PlayerDataStruct.HPIndex++;
			Owner->PlayerHUD->SetHP(1.0f);
			return true;
		}
		return false;
	};
	HpStatList[1].Func = [&]()-> bool
	{
		if (Owner->CanActivate(HpStatList[Owner->PlayerDataStruct.HPIndex].SoulCost))
		{
			Owner->PlayerDataStruct.CharacterMaxHp += Owner->PlayerDataStruct.CharacterMaxHp * HpStatList[Owner->PlayerDataStruct.HPIndex].Value;
			Owner->PlayerDataStruct.HPIndex++;
			Owner->PlayerHUD->SetHP(1.0f);
			return true;
		}
		return false;
	};
	HpStatList[2].Func = [&]()-> bool
	{
		if (Owner->CanActivate(HpStatList[Owner->PlayerDataStruct.HPIndex].SoulCost))
		{
			Owner->PlayerDataStruct.CharacterMaxHp += Owner->PlayerDataStruct.CharacterMaxHp * HpStatList[Owner->PlayerDataStruct.HPIndex].Value;
			Owner->PlayerDataStruct.HPIndex++;
			Owner->PlayerHUD->SetHP(1.0f);
			return true;
		}
		return false;
	};
	HpStatList[3].Func = [&]()-> bool
	{
		if (Owner->CanActivate(HpStatList[Owner->PlayerDataStruct.HPIndex].SoulCost))
		{
			Owner->PlayerDataStruct.PlayerHealValue += Owner->PlayerDataStruct.PlayerHealValue * HpStatList[Owner->PlayerDataStruct.HPIndex].Value;
			Owner->PlayerDataStruct.HPIndex++;
			return true;
		}
		return false;
	};
	HpStatList[4].Func = [&]()-> bool
	{
		if (Owner->CanActivate(HpStatList[Owner->PlayerDataStruct.HPIndex].SoulCost))
		{
			Owner->PlayerDataStruct.MaxHealCount += HpStatList[Owner->PlayerDataStruct.HPIndex].Value;
			Owner->CurHealCount = Owner->PlayerDataStruct.MaxHealCount;
			Owner->PlayerDataStruct.HPIndex++;
			return true;
		}
		return false;
	};

	ShieldStatList[0].Func = [&]()-> bool
	{
		if (Owner->CanActivate(ShieldStatList[Owner->PlayerDataStruct.ShieldIndex].SoulCost))
		{
			Owner->PlayerDataStruct.SoulBonusCount += ShieldStatList[Owner->PlayerDataStruct.ShieldIndex].Value;
			Owner->PlayerDataStruct.ShieldIndex++;
			return true;
		}
		return false;
	};
	ShieldStatList[1].Func = [&]()-> bool
	{
		if (Owner->CanActivate(ShieldStatList[Owner->PlayerDataStruct.ShieldIndex].SoulCost))
		{
			Owner->PlayerDataStruct.SoulBonusCount += ShieldStatList[Owner->PlayerDataStruct.ShieldIndex].Value;
			Owner->PlayerDataStruct.ShieldIndex++;
			return true;
		}
		return false;
	};
	ShieldStatList[2].Func = [&]()-> bool
	{
		if (Owner->CanActivate(ShieldStatList[Owner->PlayerDataStruct.ShieldIndex].SoulCost))
		{
			Owner->PlayerDataStruct.SoulBonusCount += ShieldStatList[Owner->PlayerDataStruct.ShieldIndex].Value;
			Owner->PlayerDataStruct.ShieldIndex++;
			return true;
		}
		return false;
	};
	ShieldStatList[3].Func = [&]()-> bool
	{
		if (Owner->CanActivate(ShieldStatList[Owner->PlayerDataStruct.ShieldIndex].SoulCost))
		{
			Owner->PlayerDataStruct.SkillSoulCost = ShieldStatList[Owner->PlayerDataStruct.ShieldIndex].Value;
			Owner->PlayerDataStruct.ShieldIndex++;
			return true;
		}
		return false;
	};
	ShieldStatList[4].Func = [&]()-> bool
	{
		if (Owner->CanActivate(ShieldStatList[Owner->PlayerDataStruct.ShieldIndex].SoulCost))
		{
			Owner->PlayerDataStruct.ShieldDecreaseSoulPercent = ShieldStatList[Owner->PlayerDataStruct.ShieldIndex].Value;
			Owner->PlayerDataStruct.ShieldBashSoulCost -= ShieldStatList[Owner->PlayerDataStruct.ShieldIndex].Value * 10.0f;
			Owner->PlayerDataStruct.ShieldIndex++;
			return true;
		}
		return false;
	};
}