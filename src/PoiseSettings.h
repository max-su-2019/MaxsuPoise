#pragma once

namespace MaxsuPoise
{
	class PoiseSettings
	{
	public:
		static void Add();

	private:
		PoiseSettings() = delete;
		~PoiseSettings() = delete;

		static RE::Setting BasePoiseHealth;
		static RE::Setting BaseArmorPoiseHealth;
		static RE::Setting HeavyArmorPoiseBonus;
		static RE::Setting BaseWeaponPoiseDamage;
		static RE::Setting BaseMagicPoiseDamage;
		static RE::Setting BlockingMult;
		static RE::Setting SmallStaggerTHLD;
		static RE::Setting MediumStaggerTHLD;
		static RE::Setting LargeStaggerTHLD;
		static RE::Setting PoiseRegen;
		static RE::Setting RegenDelayTime;
	};

}