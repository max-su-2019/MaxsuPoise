#include "Utils.h"

namespace MaxsuPoise
{
	std::optional<float> GetGameSettingFloat(const std::string a_name)
	{
		std::optional<float> result;

		auto setting = RE::GameSettingCollection::GetSingleton()->GetSetting(a_name.c_str());
		if (setting) {
			result.emplace(setting->GetFloat());
		}

		return result;
	}

	float GetGameSettingFloat(const std::string a_name, const float a_default)
	{
		auto setting = RE::GameSettingCollection::GetSingleton()->GetSetting(a_name.c_str());
		if (setting) {
			return setting->GetFloat();
		}

		return a_default;
	}
}
