#include "SettingsHandler.h"
#include "SimpleIni.h"

namespace MaxsuPoise
{
	static constexpr char fileName[] = R"(Data\SKSE\Plugins\MaxsuPoise.ini)";

	bool SettingsHandler::Register()
	{
		UpdateWeapTypeMult();
		InitArmorSlotMult();

		static SettingsHandler singleton;
		auto eventSource = SKSE::GetModCallbackEventSource();
		if (!eventSource) {
			ERROR("EventSource not found!");
			return false;
		}

		eventSource->AddEventSink(&singleton);

		INFO("Register {}", typeid(singleton).name());

		return true;
	}

	void SettingsHandler::UpdateWeapTypeMult()
	{
		auto& WeapTypeEnumTbl = dku::static_enum<WEAPON_TYPE>();

		CSimpleIniA ini;
		ini.LoadFile(fileName);  // Load the ini file

		// get a pointer to the "WeaponTypeMult" section
		const CSimpleIniA::TKeyVal* section = ini.GetSection("WeaponTypeMult");

		// iterate through the key-value pairs in the section
		for (CSimpleIniA::TKeyVal::const_iterator it = section->begin(); it != section->end(); ++it) {
			const char* key = it->first.pItem;
			const char* value = it->second;
			auto weapEnum = WeapTypeEnumTbl.from_string(key);
			if (weapEnum.has_value()) {
				weapTypeMultMap[weapEnum.value()] = (std::stof(value));
			}
		}
	}

	void SettingsHandler::InitArmorSlotMult()
	{
		static constexpr char armorSlotFile[] = R"(Data\SKSE\Plugins\MaxsuPoise_ArmorSlot.ini)";

		auto& BipeSlotEnumTbl = dku::static_enum<BipedSlot>();

		CSimpleIniA ini;
		ini.LoadFile(armorSlotFile);  // Load the ini file

		// get a pointer to the "ArmorSlotMult" section
		const CSimpleIniA::TKeyVal* section = ini.GetSection("ArmorSlotMult");

		// iterate through the key-value pairs in the section
		for (CSimpleIniA::TKeyVal::const_iterator it = section->begin(); it != section->end(); ++it) {
			const char* key = it->first.pItem;
			const char* value = it->second;
			auto armorSlotEnum = BipeSlotEnumTbl.from_string(key);
			if (armorSlotEnum.has_value()) {
				armorSlotMultMap[armorSlotEnum.value()] = (std::stof(value));
			}
		}
	}

}
