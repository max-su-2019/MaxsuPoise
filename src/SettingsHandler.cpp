#include "SettingsHandler.h"
#include "SimpleIni.h"

namespace MaxsuPoise
{
	static constexpr char FileName[] = R"(Data\SKSE\Plugins\MaxsuPoise.ini)";

	static DKUtil::model::enumeration<WEAPON_TYPE, std::uint32_t> WeapTypeEnumTbl;

	bool SettingsHandler::Register()
	{
		UpdateWeapTypeMult();

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
		CSimpleIniA ini;
		ini.LoadFile(FileName);  // Load the ini file

		// get a pointer to the "WeaponTypeMult" section
		const CSimpleIniA::TKeyVal* section = ini.GetSection("WeaponTypeMult");

		// iterate through the key-value pairs in the section
		for (CSimpleIniA::TKeyVal::const_iterator it = section->begin(); it != section->end(); ++it) {
			const char* key = it->first.pItem;
			const char* value = it->second;
			auto weapEnum = WeapTypeEnumTbl.from_string(key);
			if (weapEnum.has_value()) {
				SettingsHandler::WeapTypeMultMap[weapEnum.value()] = (std::stof(value));
			}
		}
	}

}
