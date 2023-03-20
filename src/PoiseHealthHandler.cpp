#include "PoiseHealthHandler.h"

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

		auto actorPoiseHealth = GetBasePoiseHealth() * GetBaseMass(a_target) * a_target->GetScale();
		auto armorPoiseHealth = GetTotalArmorPoiseHealth(a_target);
		return actorPoiseHealth + armorPoiseHealth;
	}

	float PoiseHealthHandler::GetBasePoiseHealth()
	{
		return 100.0f;
	}

	float PoiseHealthHandler::GetBaseMass(RE::Actor* a_target)
	{
		if (!a_target || !a_target->race)
			return 0.f;

		return a_target->race->data.baseMass;
	}

	float PoiseHealthHandler::GetTotalArmorPoiseHealth(RE::Actor* a_target)
	{
		float result = 0.f;

		if (!a_target)
			return result;

		using BipedSlot = RE::BipedObjectSlot;
		static BipedSlot armorSlots[] = {
			BipedSlot::kBody,
			BipedSlot::kFeet,
			BipedSlot::kHands,
			BipedSlot::kHead
		};

		auto baseArmorPoiseHealth = GetBaseArmorPoiseHealth();
		auto heavyArmorBouns = GetHeavyArmorBouns();

		for (const auto& slot : armorSlots) {
			auto armor = a_target->GetWornArmor(slot);
			if (armor && (armor->IsLightArmor() || armor->IsHeavyArmor())) {
				result += armor->IsLightArmor() ? baseArmorPoiseHealth : baseArmorPoiseHealth * (1 + heavyArmorBouns);
			}
		}

		return result;
	}

	float PoiseHealthHandler::GetBaseArmorPoiseHealth()
	{
		return 2.5f;
	}

	float PoiseHealthHandler::GetHeavyArmorBouns()
	{
		return 0.2f;
	}

}