#include "PoiseDamageCalculator.h"
#include "SettingsHandler.h"
#include "Utils.h"

namespace MaxsuPoise
{
	float PoiseDamageCalculator::GetWeaponPoiseDamage(const RE::HitData* a_hitData)
	{
		float result = 0.f;
		auto target = a_hitData->target ? a_hitData->target.get().get() : nullptr;
		if (!target)
			return result;

		auto aggressor = a_hitData->aggressor ? a_hitData->aggressor.get().get() : nullptr;
		auto sourceProjectile = a_hitData->sourceRef ? a_hitData->sourceRef.get().get()->AsProjectile() : nullptr;

		float baseWeapDamage = sourceProjectile ? GetBaseRangePoiseDamage() : GetBaseMeleePoiseDamage();
		float weapDamageMult = sourceProjectile ? GetWeaponDamageMult(sourceProjectile->GetProjectileRuntimeData().weaponSource) : GetWeaponDamageMult(a_hitData->weapon);
		float animDamageMult = aggressor ? GetAnimationDamageMult(aggressor) : 0.f;
		float attackDataMult = GetAttackDataDamageMult(a_hitData->attackData.get());
		float ModTargetStagger = GetPerkModTargetStagger(aggressor, target);
		float ModIncomingStagger = GetPerkModIncomingStagger(aggressor, target);
		float StrengthMult = GetStrengthMult(aggressor, target);
		float BlockingMult = GetBlockingMult(a_hitData);

		result = baseWeapDamage * (weapDamageMult + StrengthMult + attackDataMult) * (1 + animDamageMult) * ModTargetStagger * ModIncomingStagger;
		if (a_hitData->flags.any(RE::HitData::Flag::kBlocked)) {
			result *= BlockingMult;
		}

		return result;
	}

	float PoiseDamageCalculator::GetBaseMeleePoiseDamage()
	{
		return GetGameSettingFloat("fMaxsuPoise_BaseMeleePoiseDamage", 10.5f);
	}

	float PoiseDamageCalculator::GetBaseRangePoiseDamage()
	{
		return GetGameSettingFloat("fMaxsuPoise_BaseRangePoiseDamage", 10.5f);
	}

	float PoiseDamageCalculator::GetWeaponDamageMult(RE::TESObjectWEAP* a_weapon)
	{
		if (!a_weapon)
			return 0.f;

		if (a_weapon->HasKeywordString("MaxsuPoise_UniqueWeapStagger"))
			return a_weapon->GetStagger();

		auto weapType = a_weapon->GetWeaponType();
		auto item = SettingsHandler::weapTypeMultMap.find(weapType);
		if (item != SettingsHandler::weapTypeMultMap.end()) {
			return item->second;
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

	float PoiseDamageCalculator::GetBlockingMult(const RE::HitData* a_hitData)
	{
		static enum BlockedModes {
			kPercentBlocked = 0,
			kFullyBlocked = 1
		};

		auto blockedMode = GetGameSettingUInt("uMaxsuPoise_BlockedMode", 0);
		if (a_hitData && blockedMode == BlockedModes::kPercentBlocked)
			return (1.f - std::clamp(a_hitData->percentBlocked, 0.f, 1.f));

		return 0.f;
	}

	float PoiseDamageCalculator::GetStrengthMult(RE::Actor* a_aggressor, RE::Actor* a_target)
	{
		float attackerSTRG = GetActorMass(a_aggressor) * a_aggressor->GetScale();
		float targetSTRG = GetActorMass(a_target) * a_target->GetScale();
		return attackerSTRG / targetSTRG;
	}

	float PoiseDamageCalculator::GetMagicPoiseDamage(RE::Actor* a_target, float a_staggerMult, RE::Actor* a_aggressor)
	{
		if (!a_target)
			return 0.0f;

		auto baseMagicDamage = GetBaseMagicPoiseDamage();
		float ModTargetStagger = GetPerkModTargetStagger(a_aggressor, a_target);
		float ModIncomingStagger = GetPerkModIncomingStagger(a_aggressor, a_target);

		return baseMagicDamage * a_staggerMult * ModTargetStagger * ModIncomingStagger;
	}

	float PoiseDamageCalculator::GetBaseMagicPoiseDamage()
	{
		return GetGameSettingFloat("fMaxsuPoise_BaseMagicPoiseDamage", 100.0f);
	}

	float PoiseDamageCalculator::GetPerkModTargetStagger(RE::Actor* a_aggressor, RE::Actor* a_target)
	{
		using EntryPoint = RE::BGSEntryPointPerkEntry::EntryPoint;
		float result = 1.0f;
		ApplyPerkEntryPoint(EntryPoint::kModTargetStagger, a_aggressor, a_target, &result);
		return result;
	}

	float PoiseDamageCalculator::GetPerkModIncomingStagger(RE::Actor* a_aggressor, RE::Actor* a_target)
	{
		using EntryPoint = RE::BGSEntryPointPerkEntry::EntryPoint;
		float result = 1.0f;
		ApplyPerkEntryPoint(EntryPoint::kModIncomingStagger, a_target, a_aggressor, &result);
		return result;
	}
}
