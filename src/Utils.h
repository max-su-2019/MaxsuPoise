#pragma once
#include "AnnoInfo.h"

namespace MaxsuPoise
{

	float GetGameSettingFloat(const std::string a_name, const float a_default);

	std::uint32_t GetGameSettingUInt(const std::string a_name, const std::uint32_t a_default);

	bool GetGameSettingBool(const std::string a_name, const bool a_default);

	float GetActorMass(RE::Actor* a_target);

	void CPrint(const char* a_fmt, ...);

	void ApplyPerkEntryPoint(INT32 entry, RE::Actor* actor_a, RE::Actor* actor_b, float* out);

	RE::hkbClipGenerator* ToClipGenerator(RE::hkbNode* a_node);

}