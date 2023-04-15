#include "SettingsHandler.h"
#include "nlohmann/json.hpp"

namespace MaxsuPoise
{
	using json = nlohmann::json;
	static constexpr char WeapTypeFileName[] = R"(Data\SKSE\Plugins\MaxsuPoise\MaxsuPoise_WeapTypeMult.json)";

	static DKUtil::model::enumeration<WEAPON_TYPE, std::uint32_t> WeapTypeEnumTbl;

	SettingsHandler::SettingsHandler()
	{
		try {
			auto js = json::parse(std::ifstream(WeapTypeFileName));
			for (auto& [key, value] : js.items()) {
				auto enumType = WeapTypeEnumTbl.from_string(key);
				if (!enumType.has_value()) {
					continue;
				}

				WeapTypeMultMap.insert({ enumType.value(), value.get<float>() });
			}
		} catch (const json::exception& ex) {
			ERROR("Caught expection when parsing file \"{}\":\n {}", WeapTypeFileName, ex.what());
		}
	}
}
