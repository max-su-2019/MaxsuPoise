#pragma once
#include "DKUtil/Config.hpp"

namespace MaxsuPoise
{
	using WEAPON_TYPE = RE::WEAPON_TYPE;
	using EventResult = RE::BSEventNotifyControl;

	class SettingsHandler : public RE::BSTEventSink<SKSE::ModCallbackEvent>
	{
	public:
		static inline std::map<WEAPON_TYPE, float> WeapTypeMultMap;
		static bool Register();
		static constexpr char modName[] = "MaxsuPoise";

		virtual EventResult ProcessEvent(const SKSE::ModCallbackEvent* a_event, RE::BSTEventSource<SKSE::ModCallbackEvent>* a_eventSource)
		{
			if (a_event && _strcmpi(a_event->eventName.c_str(), "dmenu_updateSettings") == 0 && _strcmpi(a_event->strArg.c_str(), modName) == 0) {
				UpdateWeapTypeMult();
			}

			return EventResult::kContinue;
		}

	private:
		SettingsHandler() = default;

		static void UpdateWeapTypeMult();
	};

}