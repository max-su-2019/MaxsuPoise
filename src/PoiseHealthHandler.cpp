#include "PoiseHealthHandler.h"
#include "SettingsHandler.h"
#include "Utils.h"

namespace MaxsuPoise
{
	static constexpr char CURRENT_POISE_HEALTH_GV[] = "MaxsuPoise_PoiseHealth";

	float PoiseHealthHandler::GetCurrentPoiseHealth(RE::Actor* a_target)
	{
		float result = 0.f;
		if (!a_target || !a_target->GetGraphVariableFloat(CURRENT_POISE_HEALTH_GV, result))
			WARN("Not Graph Variable Float Found: {}", CURRENT_POISE_HEALTH_GV);

		return result;
	}

	bool PoiseHealthHandler::SetCurrentPoiseHealth(RE::Actor* a_target, const float& a_in)
	{
		return a_target && a_target->SetGraphVariableFloat(CURRENT_POISE_HEALTH_GV, a_in);
	}

	float PoiseHealthHandler::GetTotalPoiseHealth(RE::Actor* a_target)
	{
		if (!a_target)
			return 0.f;

		auto actorPoiseHealth = GetBasePoiseHealth() * GetActorMass(a_target) * a_target->GetScale();
		auto armorPoiseHealth = GetTotalArmorPoiseHealth(a_target);
		return actorPoiseHealth + armorPoiseHealth;
	}

	float PoiseHealthHandler::GetBasePoiseHealth()
	{
		return GetGameSettingFloat("fMaxsuPoise_BasePoiseHealth", 40.f);
	}

	float PoiseHealthHandler::GetTotalArmorPoiseHealth(RE::Actor* a_target)
	{
		float result = 0.f;

		if (!a_target)
			return result;

		auto baseArmorPoiseHealth = GetBaseArmorPoiseHealth();
		auto heavyArmorBouns = GetHeavyArmorBouns();

		for (const auto& pair : SettingsHandler::armorSlotMultMap) {
			auto slot = pair.first;
			auto armor = a_target->GetWornArmor(slot);
			auto armorHealth = baseArmorPoiseHealth * pair.second;
			if (armor && (armor->IsLightArmor() || armor->IsHeavyArmor())) {
				result += armor->IsLightArmor() ? armorHealth : armorHealth * (1 + heavyArmorBouns);
			} else {
				armor = a_target->GetSkin(slot);
				if (armor && (armor->IsLightArmor() || armor->IsHeavyArmor()))
					result += armor->IsLightArmor() ? armorHealth : armorHealth * (1 + heavyArmorBouns);
			}
		}

		return result;
	}

	float PoiseHealthHandler::GetBaseArmorPoiseHealth()
	{
		return GetGameSettingFloat("fMaxsuPoise_BaseArmorPoiseHealth", 50.f);
	}

	float PoiseHealthHandler::GetHeavyArmorBouns()
	{
		return GetGameSettingFloat("fMaxsuPoise_HeavyArmorPoiseBonus", 0.5f);
	}
}