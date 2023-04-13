#pragma once

namespace MaxsuPoise
{
	class PoiseDamageCalculator
	{
	public:
		static float GetPhysicalPoiseDamage(const RE::HitData* a_hitData);

	private:
		static float GetBaseWeaponPoiseDamage();
		static float GetWeaponDamageMult(RE::TESObjectWEAP* a_weapon);
		static float GetAnimationDamageMult(RE::Actor* a_aggressor);
		static float GetAttackDataDamageMult(RE::BGSAttackData* a_attackData);
		static float GetPerkModTargetStagger(RE::Actor* a_aggressor, RE::Actor* a_target);
		static float GetPerkModIncomingStagger(RE::Actor* a_aggressor, RE::Actor* a_target);
		static float GetBlockingMult();
		static float GetStrengthMult(RE::Actor* a_aggressor, RE::Actor* a_target);

		PoiseDamageCalculator() = delete;

		~PoiseDamageCalculator() = delete;
	};

}