#pragma once
#include "StaggerHandler.h"
#include "nlohmann/json.hpp"

namespace MaxsuPoise
{
	using json = nlohmann::json;

	struct AnnoInfo
	{
		float startTime = -1.f;
		float duration = 0.0f;
		float damageMult = 1.f;
		StaggerLevel immuneLevel = StaggerLevel::kNone;

		friend void from_json(const json& j, AnnoInfo& a_annoInfo);
		bool IsInFrames(const float a_localTime) const;
	};

	void from_json(const json& j, AnnoInfo& a_annoInfo);
}