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

	float GetActorMass(RE::Actor* a_target)
	{
		if (!a_target || !a_target->race)
			return 0.f;

		return a_target->race->data.baseMass;
	}

	void CPrint(const char* a_fmt, ...)
	{
		auto console = RE::ConsoleLog::GetSingleton();
		if (console) {
			std::va_list args;
			va_start(args, a_fmt);
			console->VPrint(a_fmt, args);
			va_end(args);
		}
	}

	void ApplyPerkEntryPoint(INT32 entry, RE::Actor* actor_a, RE::Actor* actor_b, float* out)
	{
		using func_t = decltype(&ApplyPerkEntryPoint);
		REL::Relocation<func_t> func{ REL::RelocationID(23073, 23526) };  // 1.5.97 14032ECE0
		return func(entry, actor_a, actor_b, out);
	}
}
