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

		if (!a_target->IsInCombat() || currentPoiseHealth > totalPoiseHealth) {
			currentPoiseHealth = totalPoiseHealth;
		} else if (!a_target->IsStaggering()) {
			auto regenDelayTimer = GetPoiseRegenDelayTimer(a_target);
			if (regenDelayTimer > 0.f)
				SetPoiseRegenDelayTimer(a_target, regenDelayTimer - a_delta);
			else {
				currentPoiseHealth > 0.f ?
					currentPoiseHealth += totalPoiseHealth * a_delta * GetPoiseRegenRate() :
					currentPoiseHealth = totalPoiseHealth;
			}
		}

		PoiseHealthHandler::SetCurrentPoiseHealth(a_target, currentPoiseHealth);
	}

	float PoiseRegenHandler::GetPoiseRegenRate()
	{
		return GetGameSettingFloat("fMaxsuPoise_PoiseRegen", 0.3f);
	}

	float PoiseRegenHandler::GetMaxRegenDelayTime()
	{
		return GetGameSettingFloat("fMaxsuPoise_RegenDelayTime", 1.5f);
	}

	static constexpr char CURRENT_REGEN__DELAYGV[] = "MaxsuPoise_RegenDelayTimer";

	float PoiseRegenHandler::GetPoiseRegenDelayTimer(RE::Actor* a_target)
	{
		float result = 0.f;
		if (!a_target || !a_target->GetGraphVariableFloat(CURRENT_REGEN__DELAYGV, result))
			WARN("Not Graph Variable Float Found: {}", CURRENT_REGEN__DELAYGV);

		return result;
	}

	float PoiseRegenHandler::SetPoiseRegenDelayTimer(RE::Actor* a_target, const float& a_in)
	{
		return a_target && a_target->SetGraphVariableFloat(CURRENT_REGEN__DELAYGV, a_in);
	}

}