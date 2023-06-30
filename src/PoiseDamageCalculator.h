#pragma once

namespace MaxsuPoise
{
	class PoiseDamageCalculator
	{
	public:
		static float GetWeaponPoiseDamage(const RE::HitData* a_hitData);
		static float GetMagicPoiseDamage(RE::Actor* a_target, float a_staggerMult, RE::Actor* a_aggressor);

	private:
		static float GetBaseMeleePoiseDamage();
		static float GetBaseRangePoiseDamage();
		static float GetWeaponDamageMult(RE::TESObjectWEAP* a_weapon);
		static float GetAnimationDamageMult(RE::Actor* a_aggressor);
		static float GetAttackDataDamageMult(RE::BGSAttackData* a_attackData);
		static float GetBlockingMult(const RE::HitData* a_hitData);
		static float GetStrengthMult(RE::Actor* a_aggressor, RE::Actor* a_target);

		static float GetBaseMagicPoiseDamage();

		static float GetPerkModTargetStagger(RE::Actor* a_aggressor, RE::Actor* a_target);
		static float GetPerkModIncomingStagger(RE::Actor* a_aggressor, RE::Actor* a_target);

		PoiseDamageCalculator() = delete;

		~PoiseDamageCalculator() = delete;
	};

}