#pragma once

namespace MaxsuPoise
{
	class PoiseHealthHandler
	{
	public:
		static float GetTotalPoiseHealth(RE::Actor* a_target);
		static float GetCurrentPoiseHealth(RE::Actor* a_target);
		static bool SetCurrentPoiseHealth(RE::Actor* a_target, const float& a_in);

	private:
		static float GetBasePoiseHealth();
		static float GetBaseMass(RE::Actor* a_target);

		static float GetTotalArmorPoiseHealth(RE::Actor* a_target);
		static float GetBaseArmorPoiseHealth();
		static float GetHeavyArmorBouns();

		PoiseHealthHandler() = delete;

		~PoiseHealthHandler() = delete;
	};

}