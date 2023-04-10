#include "PoiseRegenHandler.h"
#include "PoiseHealthHandler.h"
#include "Utils.h"

namespace MaxsuPoise
{
	void PoiseRegenHandler::CharacterEx::Hook_Update(float a_delta)
	{
		func(this, a_delta);
		UpdatePoiseValue(this, RE::GetSecondsSinceLastFrame());
	}

	void PoiseRegenHandler::UpdatePoiseValue(RE::Actor* a_target, float a_delta)
	{
		if (!a_target || !a_target->currentProcess || !a_target->currentProcess->high)
			return;

		const auto totalPoiseHealth = PoiseHealthHandler::GetTotalPoiseHealth(a_target);
		auto currentPoiseHealth = PoiseHealthHandler::GetCurrentPoiseHealth(a_target);

		if (!a_target->IsInCombat() || (currentPoiseHealth - totalPoiseHealth) >= 0.01f) {
			currentPoiseHealth = totalPoiseHealth;
		} else if (!a_target->IsStaggering()) {
			auto regenDelayTimer = RegenDelayHandler::GetPoiseRegenDelayTimer(a_target);
			if (regenDelayTimer > 0.f)
				RegenDelayHandler::SetPoiseRegenDelayTimer(a_target, regenDelayTimer - a_delta);
			else {
				currentPoiseHealth += totalPoiseHealth * a_delta * GetPoiseRegenRate();
			}

			auto staggerProtectTimer = StaggerProtectHandler::GetStaggerProtectTimer(a_target);
			if (staggerProtectTimer > 0.f)
				StaggerProtectHandler::SetStaggerProtectTimer(a_target, staggerProtectTimer - a_delta);
		}

		PoiseHealthHandler::SetCurrentPoiseHealth(a_target, currentPoiseHealth);
	}

	float PoiseRegenHandler::GetPoiseRegenRate()
	{
		return GetGameSettingFloat("fMaxsuPoise_PoiseRegen", 0.05f);
	}

	float RegenDelayHandler::GetMaxRegenDelayTime()
	{
		return GetGameSettingFloat("fMaxsuPoise_RegenDelayTime", 1.0f);
	}

	float RegenDelayHandler::GetPoiseRegenDelayTimer(RE::Actor* a_target)
	{
		float result = 0.f;
		if (!a_target || !a_target->GetGraphVariableFloat(CURRENT_REGEN_DELAY_GV, result))
			WARN("Not Graph Variable Float Found: {}", CURRENT_REGEN_DELAY_GV);

		return result;
	}

	bool RegenDelayHandler::SetPoiseRegenDelayTimer(RE::Actor* a_target, const float& a_in)
	{
		return a_target && a_target->SetGraphVariableFloat(CURRENT_REGEN_DELAY_GV, a_in);
	}

	float StaggerProtectHandler::GetStaggerProtectTimer(RE::Actor* a_target)
	{
		float result = 0.f;
		if (!a_target || !a_target->GetGraphVariableFloat(CURRENT_STAGGER_TIMER_GV, result))
			WARN("Not Graph Variable Float Found: {}", CURRENT_STAGGER_TIMER_GV);

		return result;
	}

	bool StaggerProtectHandler::SetStaggerProtectTimer(RE::Actor* a_target, const float& a_in)
	{
		return a_target && a_target->SetGraphVariableFloat(CURRENT_STAGGER_TIMER_GV, a_in);
	}

	float StaggerProtectHandler::GetMaxStaggerProtectTime()
	{
		return GetGameSettingFloat("fMaxsuPoise_StaggerProtectTime", 0.85f);
	}
}