#include "PoiseDamageCalculator.h"
#include "Utils.h"

namespace MaxsuPoise
{
	float PoiseDamageCalculator::GetPhysicalPoiseDamage(const RE::HitData* a_hitData)
	{
		float result = 0.f;
		auto target = a_hitData->target ? a_hitData->target.get().get() : nullptr;
		if (!target)
			return result;

		auto aggressor = a_hitData->aggressor ? a_hitData->aggressor.get().get() : nullptr;

		float baseWeapDamage = GetBaseWeaponPoiseDamage();
		float weapDamageMult = GetWeaponDamageMult(a_hitData->weapon);
		float animDamageMult = aggressor ? GetAnimationDamageMult(aggressor) : 0.f;
		float attackDataMult = GetAttackDataDamageMult(a_hitData->attackData.get());
		float ModTargetStagger = GetPerkModTargetStagger(aggressor);
		float ModIncomingStagger = GetPerkModIncomingStagger(target);
		float StrengthMult = GetStrengthMult(aggressor, target);

		result = baseWeapDamage * (weapDamageMult + StrengthMult) * (animDamageMult + attackDataMult + ModTargetStagger * ModIncomingStagger);
		if (a_hitData->flags.any(RE::HitData::Flag::kBlocked)) {
			result *= GetBlockingMult();
		}

		return result;
	}

	float PoiseDamageCalculator::GetBaseWeaponPoiseDamage()
	{
		return GetGameSettingFloat("fMaxsuPoise_BaseWeaponPoiseDamage", 20.0f);
	}

	float PoiseDamageCalculator::GetWeaponDamageMult(RE::TESObjectWEAP* a_weapon)
	{
		if (!a_weapon)
			return 0.f;

		switch (a_weapon->GetWeaponType()) {
		case RE::WEAPON_TYPE::kOneHandSword:
		case RE::WEAPON_TYPE::kOneHandAxe:
		case RE::WEAPON_TYPE::kOneHandMace:
			return 1.0f;

		case RE::WEAPON_TYPE::kOneHandDagger:
			return 0.5f;

		case RE::WEAPON_TYPE::kTwoHandAxe:
		case RE::WEAPON_TYPE::kTwoHandSword:
			return 1.5f;

		default:
			break;
		}

		return 0.0f;
	}

	float PoiseDamageCalculator::GetAnimationDamageMult(RE::Actor* a_aggressor)
	{
		return 0.0f;
	}

	float PoiseDamageCalculator::GetAttackDataDamageMult(RE::BGSAttackData* a_attackData)
	{
		if (!a_attackData)
			return 0.0f;

		return a_attackData->data.staggerOffset;
	}

	float PoiseDamageCalculator::GetPerkModTargetStagger(RE::Actor* a_aggressor)
	{
		return 1.0f;
	}

	float PoiseDamageCalculator::GetPerkModIncomingStagger(RE::Actor* a_target)
	{
		return 1.0f;
	}

	float PoiseDamageCalculator::GetBlockingMult()
	{
		return GetGameSettingFloat("fMaxsuPoise_BlockingMult", 0.0f);
	}

	float PoiseDamageCalculator::GetStrengthMult(RE::Actor* a_aggressor, RE::Actor* a_target)
	{
		float attackerSTRG = GetActorMass(a_aggressor) * a_aggressor->GetScale();
		float targetSTRG = GetActorMass(a_target) * a_target->GetScale();
		return attackerSTRG / targetSTRG;
	}

}