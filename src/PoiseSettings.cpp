#include "PoiseSettings.h"

namespace MaxsuPoise
{
	void PoiseSettings::Add()
	{
		auto* gameSettingCollection = RE::GameSettingCollection::GetSingleton();
		if (gameSettingCollection) {
			gameSettingCollection->InsertSetting(std::addressof(BasePoiseHealth));
			gameSettingCollection->InsertSetting(std::addressof(BaseArmorPoiseHealth));
			gameSettingCollection->InsertSetting(std::addressof(HeavyArmorPoiseBonus));
			gameSettingCollection->InsertSetting(std::addressof(BaseWeaponPoiseDamage));
			gameSettingCollection->InsertSetting(std::addressof(BaseMagicPoiseDamage));
			gameSettingCollection->InsertSetting(std::addressof(BlockingMult));
			gameSettingCollection->InsertSetting(std::addressof(SmallStaggerTHLD));
			gameSettingCollection->InsertSetting(std::addressof(MediumStaggerTHLD));
			gameSettingCollection->InsertSetting(std::addressof(LargeStaggerTHLD));
			gameSettingCollection->InsertSetting(std::addressof(PoiseRegen));
			gameSettingCollection->InsertSetting(std::addressof(RegenDelayTime));
		}
	}

	RE::Setting PoiseSettings::BasePoiseHealth("fMaxsuPoise_BasePoiseHealth", 100.f);
	RE::Setting PoiseSettings::BaseArmorPoiseHealth("fMaxsuPoise_BaseArmorPoiseHealth", 2.5f);
	RE::Setting PoiseSettings::HeavyArmorPoiseBonus("fMaxsuPoise_HeavyArmorPoiseBonus", 1.5f);
	RE::Setting PoiseSettings::BaseWeaponPoiseDamage("fMaxsuPoise_BaseWeaponPoiseDamage", 20.f);
	RE::Setting PoiseSettings::BaseMagicPoiseDamage("fMaxsuPoise_BaseMagicPoiseDamage", 50.f);
	RE::Setting PoiseSettings::BlockingMult("fMaxsuPoise_BlockingMult", 0.f);
	RE::Setting PoiseSettings::SmallStaggerTHLD("fMaxsuPoise_SmallStaggerTHLD", 0.15f);
	RE::Setting PoiseSettings::MediumStaggerTHLD("fMaxsuPoise_MediumStaggerTHLD", 0.3f);
	RE::Setting PoiseSettings::LargeStaggerTHLD("fMaxsuPoise_LargeStaggerTHLD", 0.5f);
	RE::Setting PoiseSettings::PoiseRegen("fMaxsuPoise_PoiseRegen", 0.3f);
	RE::Setting PoiseSettings::RegenDelayTime("fMaxsuPoise_RegenDelayTime", 1.5f);
}
