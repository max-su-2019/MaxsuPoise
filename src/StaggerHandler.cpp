#include "StaggerHandler.h"
#include "Hooks/PoiseRegenHandler.h"
#include "PoiseDamageCalculator.h"
#include "PoiseHealthHandler.h"
#include "Utils.h"

namespace MaxsuPoise
{
	static inline void TryStagger(RE::Actor* a_target, float a_staggerMult, RE::Actor* a_aggressor)
	{
		std::int32_t staggerLevelGV = 0;
		if (a_target->IsStaggering() && !a_target->GetGraphVariableInt("MSL_StaggerLevel", staggerLevelGV)) {
			float currentStaggerMagnitude = 0.f;
			if (a_target->GetGraphVariableFloat("StaggerMagnitude", currentStaggerMagnitude) && currentStaggerMagnitude - a_staggerMult >= 0.1f) {
				return;
			}
		}

		using func_t = decltype(&TryStagger);
		REL::Relocation<func_t> func{ REL::RelocationID(36700, 37710) };
		func(a_target, a_staggerMult, a_aggressor);
	}

	void StaggerHandler::ProcessWeaponStagger(const RE::HitData* a_hitData)
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
		auto poiseDamage = PoiseDamageCalculator::GetWeaponPoiseDamage(a_hitData);
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
			logs << "-------MaxsuPoise Weapon Stagger Result-------" << std::endl;

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

	void StaggerHandler::ProcessMagicStagger(RE::Actor* a_target, float a_staggerMult, RE::Actor* a_aggressor)
	{
		if (!a_target)
			return;

		if (a_staggerMult <= 0.f) {
			return;
		}

		if (a_target == a_aggressor)
			a_aggressor = nullptr;

		auto staggerProtectTime = StaggerProtectHandler::GetStaggerProtectTimer(a_target);
		if (staggerProtectTime > 0.f && a_target->IsStaggering())
			return;

		auto totalPoiseHealth = PoiseHealthHandler::GetTotalPoiseHealth(a_target);
		auto currentPoiseHealth = PoiseHealthHandler::GetCurrentPoiseHealth(a_target);
		auto poiseDamage = PoiseDamageCalculator::GetMagicPoiseDamage(a_target, a_staggerMult, a_aggressor);
		currentPoiseHealth -= poiseDamage;

		auto staggerLevel = GetStaggerLevel(poiseDamage / totalPoiseHealth);
		auto immuneLevel = ImmuneLevelCalculator::GetTotalImmuneLevel(a_target);
		if (currentPoiseHealth <= 0.f) {
			TryStagger(a_target, 1.0f, a_aggressor);
			if (a_target->IsStaggering()) {
				staggerProtectTime = StaggerProtectHandler::GetMaxStaggerProtectTime();
				StaggerProtectHandler::SetStaggerProtectTimer(a_target, staggerProtectTime);
				currentPoiseHealth = totalPoiseHealth;
			}
		} else if (staggerProtectTime <= 0.f) {
			if (staggerLevel && staggerLevel > immuneLevel) {
				TryStagger(a_target, 0.25f * (staggerLevel) + 0.01f, a_aggressor);
			}
		}

		PoiseHealthHandler::SetCurrentPoiseHealth(a_target, currentPoiseHealth);
		RegenDelayHandler::SetPoiseRegenDelayTimer(a_target, RegenDelayHandler::GetMaxRegenDelayTime());

		auto selectedRef = RE::Console::GetSelectedRef();
		if (selectedRef && a_target == selectedRef.get()) {
			std::ostringstream logs;
			logs << "-------MaxsuPoise Magic Stagger Result-------" << std::endl;

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
		float damageTHLD_Arr[StaggerLevel::kLargest] = {
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
		auto result = StaggerLevel::kNone;
		if (!a_target)
			return result;

		for (auto& pair : ImmuneKeywordMap) {
			auto keyword = RE::TESForm::LookupByEditorID<RE::BGSKeyword>(pair.first);
			if (!keyword)
				continue;

			if (a_target->HasKeyword(keyword) || HasActiveEffectWithKeyword(a_target->AsMagicTarget(), keyword)) {
				if (pair.second > result)
					result = pair.second;
			}
		}

		auto animImmuneLevel = GetAnimImmuneLevel(a_target);
		if (animImmuneLevel > result) {
			result = animImmuneLevel;
		}

		return result;
	}

	bool ImmuneLevelCalculator::HasActiveEffectWithKeyword(RE::MagicTarget* a_target, RE::BGSKeyword* a_keyword)
	{
		for (const auto effect : *(a_target->GetActiveEffectList())) {
			if (effect->GetBaseObject()->HasKeyword(a_keyword) && (effect->conditionStatus.get() == RE::ActiveEffect::ConditionStatus::kTrue))
				return true;
		}

		return false;
	}

	StaggerLevel ImmuneLevelCalculator::GetAnimImmuneLevel(RE::Actor* a_actor)
	{
		StaggerLevel result = StaggerLevel::kNone;

		if (!a_actor || !a_actor->Is3DLoaded())
			return result;

		RE::BSAnimationGraphManagerPtr graphMgr;
		if (a_actor->GetAnimationGraphManager(graphMgr) && graphMgr) {
			auto behaviourGraph = graphMgr->graphs[0] ? graphMgr->graphs[0]->behaviorGraph : nullptr;
			auto activeNodes = behaviourGraph ? behaviourGraph->activeNodes : nullptr;
			if (activeNodes) {
				for (auto nodeInfo : *activeNodes) {
					auto nodeClone = nodeInfo.nodeClone;
					if (nodeClone && nodeClone->GetClassType()) {
						auto clipGenrator = ToClipGenerator(nodeClone);
						if (clipGenrator) {
							auto immuneLevel = GetAnimImmuneLevel(clipGenrator);
							if (immuneLevel > result) {
								result = immuneLevel;
							}
						}
					}
				}
			}
		}

		return result;
	}

	StaggerLevel ImmuneLevelCalculator::GetAnimImmuneLevel(RE::hkbClipGenerator* a_clip)
	{
		StaggerLevel result = StaggerLevel::kNone;

		if (!a_clip)
			return result;

		static constexpr std::string_view prefix = "MaxsuPoise";

		auto binding = a_clip ? a_clip->binding : nullptr;
		auto animation = binding ? binding->animation : nullptr;

		if (!animation || animation->annotationTracks.empty())
			return result;

		for (auto anno : animation->annotationTracks[0].annotations) {
			std::string_view text{ anno.text.c_str() };
			if (text.starts_with(prefix)) {
				try {
					auto j = json::parse(text.substr(prefix.size()));
					auto annoInfo = j.get<AnnoInfo>();
					annoInfo.startTime = anno.time;
					if (annoInfo.IsInFrames(a_clip->localTime) && annoInfo.immuneLevel > result) {
						result = annoInfo.immuneLevel;
					}
				} catch (json::exception& ex) {
					ERROR("Caught an expection when convert annoation: {}", ex.what());
					continue;
				}
			}
		}

		return result;
	}
}
