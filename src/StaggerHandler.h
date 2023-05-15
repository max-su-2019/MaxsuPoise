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
		static bool HasActiveEffectWithKeyword(RE::MagicTarget* a_target, RE::BGSKeyword* a_keyword);

	private:
		ImmuneLevelCalculator() = delete;

		~ImmuneLevelCalculator() = delete;

		static inline std::map<std::string, StaggerLevel> ImmuneKeywordMap = {
			{ "MaxsuPoise_ImmuneSmall", StaggerLevel::kSmall },
			{ "MaxsuPoise_ImmuneMedium", StaggerLevel::kMedium },
			{ "MaxsuPoise_ImmuneLarge", StaggerLevel::kLarge },
			{ "MaxsuPoise_ImmuneLargest", StaggerLevel::kLargest }
		};
	};
}