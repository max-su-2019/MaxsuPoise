#pragma once

namespace MaxsuPoise
{
	std::optional<float> GetGameSettingFloat(const std::string a_name);

	float GetGameSettingFloat(const std::string a_name, const float a_default);

}