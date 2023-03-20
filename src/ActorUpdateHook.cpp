#include "ActorUpdateHook.h"
#include "PoiseHealthHandler.h"

namespace MaxsuPoise
{
	void CharacterEx::Hook_Update(float a_delta)
	{
		func(this, a_delta);

		const auto totalPoiseHealth = PoiseHealthHandler::GetTotalPoiseHealth(this);
		auto currentPoiseHealth = PoiseHealthHandler::GetCurrentPoiseHealth(this);

		if (!this->IsInCombat())
			currentPoiseHealth = totalPoiseHealth;
		else {
			if (currentPoiseHealth > totalPoiseHealth)
				currentPoiseHealth = totalPoiseHealth;
			else if (!this->IsStaggering()) {
				currentPoiseHealth > 0.f ? currentPoiseHealth += PoiseRegenHandler::GetPoiseRegenRate() * RE::GetSecondsSinceLastFrame() :
										   currentPoiseHealth = totalPoiseHealth;
			}
		}

		PoiseHealthHandler::SetCurrentPoiseHealth(this, currentPoiseHealth);
	}
}