#pragma once

namespace MaxsuPoise
{
	enum StaggerLevel : std::uint32_t
	{
		kNone = 0,
		kSmall,
		kMedium,
		kLarge,
		kLargest,
	};

	class StaggerHandler
	{
	public:
		static void ProcessWeaponStagger(const RE::HitData* a_hitdata);
		static void ProcessMagicStagger(RE::Actor* a_target, float a_staggerMult, RE::Actor* a_aggressor);

	private:
		static StaggerLevel GetStaggerLevel(const float& a_DamagePercent);

		StaggerHandler() = delete;

		~StaggerHandler() = delete;
	};

	class ImmuneLevelCalculator
	{
	public:
		static StaggerLevel GetTotalImmuneLevel(RE::Actor* a_target);

	private:
		ImmuneLevelCalculator() = delete;

		~ImmuneLevelCalculator() = delete;
	};
}