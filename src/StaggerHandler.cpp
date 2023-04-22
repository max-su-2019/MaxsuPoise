#include "StaggerHandler.h"
#include "PoiseDamageCalculator.h"
#include "PoiseHealthHandler.h"
#include "PoiseRegenHandler.h"
#include "Utils.h"

namespace MaxsuPoise
{
	static inline void TryStagger(RE::Actor* a_target, float a_staggerMult, RE::Actor* a_aggressor)
	{
		using func_t = decltype(&TryStagger);
		REL::Relocation<func_t> func{ REL::RelocationID(36700, 37710) };
		func(a_target, a_staggerMult, a_aggressor);
	}

	void StaggerHandler::ProcessStagger(const RE::HitData* a_hitData)
	{
		auto target = a_hitData && a_hitData->target ? a_hitData->target.get().get() : nullptr;
		if (!target)
			return;

		auto aggressor = a_hitData->aggressor ? a_hitData->aggressor.get().get() : nullptr;
		if (!aggressor)
			return;

		auto staggerProtectTime = StaggerProtectHandler::GetStaggerProtectTimer(target);
		if (staggerProtectTime > 0.f && target->IsStaggering())
			return;

		auto totalPoiseHealth = PoiseHealthHandler::GetTotalPoiseHealth(target);
		auto currentPoiseHealth = PoiseHealthHandler::GetCurrentPoiseHealth(target);
		auto poiseDamage = PoiseDamageCalculator::GetPhysicalPoiseDamage(a_hitData);
		currentPoiseHealth -= poiseDamage;

		auto staggerLevel = GetStaggerLevel(poiseDamage / totalPoiseHealth);
		auto immuneLevel = ImmuneLevelCalculator::GetTotalImmuneLevel(target);
		if (currentPoiseHealth <= 0.f) {
			TryStagger(target, 1.0f, aggressor);
			if (target->IsStaggering()) {
				staggerProtectTime = StaggerProtectHandler::GetMaxStaggerProtectTime();
				StaggerProtectHandler::SetStaggerProtectTimer(target, staggerProtectTime);
				currentPoiseHealth = totalPoiseHealth;
			}
		} else if (staggerProtectTime <= 0.f) {
			if (staggerLevel && staggerLevel > immuneLevel) {
				TryStagger(target, 0.25f * (staggerLevel) + 0.01f, aggressor);
			}
		}

		PoiseHealthHandler::SetCurrentPoiseHealth(target, currentPoiseHealth);
		RegenDelayHandler::SetPoiseRegenDelayTimer(target, RegenDelayHandler::GetMaxRegenDelayTime());

		auto selectedRef = RE::Console::GetSelectedRef();
		if (selectedRef && target == selectedRef.get()) {
			std::ostringstream logs;
			logs << "-------MaxsuPoise Stagger Result-------" << std::endl;

			if (staggerProtectTime > 0.f)
				logs << "Largest Stagger! TotalHealth: " << totalPoiseHealth << std::endl;
			else {
				logs << "TotalHealth: " << totalPoiseHealth << std::endl;
				logs << "Damage: " << poiseDamage << std::endl;
				logs << "CurrentHealth: " << currentPoiseHealth << std::endl;
				logs << "StaggerLevel: " << staggerLevel << std::endl;
				logs << "ImmuneLevel: " << immuneLevel << std::endl;
			}

			logs << "---------------------------------------" << std::endl;
			CPrint(logs.str().c_str());
		}
	}

	StaggerLevel StaggerHandler::GetStaggerLevel(const float& a_DamagePercent)
	{
		static float damageTHLD_Arr[StaggerLevel::kLargest] = {
			GetGameSettingFloat("fMaxsuPoise_SmallStaggerTHLD", 0.17f),
			GetGameSettingFloat("fMaxsuPoise_MediumStaggerTHLD", 0.25f),
			GetGameSettingFloat("fMaxsuPoise_LargeStaggerTHLD", 0.5f),
			1.0f
		};

		for (std::uint32_t i = StaggerLevel::kNone; i < StaggerLevel::kLargest; i++) {
			if (a_DamagePercent < damageTHLD_Arr[i])
				return static_cast<StaggerLevel>(i);
		}

		return StaggerLevel::kLargest;
	}

	StaggerLevel ImmuneLevelCalculator::GetTotalImmuneLevel(RE::Actor* a_target)
	{
		static std::map<std::string, StaggerLevel> ImmuneKeywordMap = {
			{ "MaxsuPoise_ImmuneSmall", StaggerLevel::kSmall },
			{ "MaxsuPoise_ImmuneMedium", StaggerLevel::kMedium },
			{ "MaxsuPoise_ImmuneLarge", StaggerLevel::kLarge },
			{ "MaxsuPoise_ImmuneLargest", StaggerLevel::kLargest }
		};

		auto result = StaggerLevel::kNone;
		if (!a_target)
			return result;

		for (auto& pair : ImmuneKeywordMap) {
			auto keyword = RE::TESForm::LookupByEditorID<RE::BGSKeyword>(pair.first);
			if (!keyword)
				continue;

			if (a_target->HasKeyword(keyword) || a_target->HasMagicEffectWithKeyword(keyword)) {
				if (pair.second > result)
					result = pair.second;
			}
		}

		return result;
	}
}
