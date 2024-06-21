#include "Utils.h"

namespace MaxsuPoise
{

	float GetGameSettingFloat(const std::string a_name, const float a_default)
	{
		auto setting = RE::GameSettingCollection::GetSingleton()->GetSetting(a_name.c_str());
		if (setting) {
			return setting->GetFloat();
		}

		return a_default;
	}

	std::uint32_t GetGameSettingUInt(const std::string a_name, const std::uint32_t a_default)
	{
		auto setting = RE::GameSettingCollection::GetSingleton()->GetSetting(a_name.c_str());
		if (setting) {
			return setting->GetUnsignedInteger();
		}

		return a_default;
	}

	float GetActorMass(RE::Actor* a_target)
	{
		if (!a_target || !a_target->race)
			return 0.f;
		
		return a_target->GetActorValue(RE::ActorValue::kMass);
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

	RE::hkbClipGenerator* ToClipGenerator(RE::hkbNode* a_node)
	{
		constexpr char CLASS_NAME[] = "hkbClipGenerator";

		if (a_node && a_node->GetClassType()) {
			if (_strcmpi(a_node->GetClassType()->name, CLASS_NAME) == 0)
				return skyrim_cast<RE::hkbClipGenerator*>(a_node);
		}

		return nullptr;
	}

}
