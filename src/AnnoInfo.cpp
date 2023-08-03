#include "AnnoInfo.h"

namespace MaxsuPoise
{
	void from_json(const json& j, AnnoInfo& a_annoInfo)
	{
		j.at("duration").get_to(a_annoInfo.duration);
		a_annoInfo.duration = std::max(a_annoInfo.duration, 0.f);

		if (j.find("damageMult") != j.end())
			a_annoInfo.damageMult = std::max(j.at("damageMult").get<float>(), 0.f);

		if (j.find("immuneLevel") != j.end())
			a_annoInfo.immuneLevel = std::clamp(j.at("immuneLevel").get<StaggerLevel>(), StaggerLevel::kNone, StaggerLevel::kLargest);
	}

	bool AnnoInfo::IsInFrames(const float a_localTime) const
	{
		return a_localTime >= startTime && a_localTime <= startTime + duration;
	}
}
