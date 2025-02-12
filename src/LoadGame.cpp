#include "LoadGame.h"
#include "Hooks/HitEventHandler.h"
#include "Hooks/MagicStaggerHook.h"
#include "Hooks/PerkEntry_Hooks.h"
#include "Hooks/PoiseRegenHandler.h"
#include "SettingsHandler.h"
#include "HUD.h"

namespace MaxsuPoise
{
	void EventCallback(SKSE::MessagingInterface::Message* msg)
	{
		if (msg->type == SKSE::MessagingInterface::kPostPostLoad) {
			static constexpr auto bdiDLLName = "BehaviorDataInjector.dll";
			auto bdiPlugin = GetModuleHandleA(bdiDLLName);
			if (!bdiPlugin) {
				ERROR("Not \"{}\" Loaded!", bdiDLLName);
				return;
			}

			MaxsuPoise::SettingsHandler::Register();
			MaxsuPoise::HitEventHandler::InstallHooks();
			MaxsuPoise::MagicStaggerHook::InstallHooks();
			MaxsuPoise::PoiseRegenHandler::CharacterEx::InstallHook();
			MaxsuPoise::PoiseRegenHandler::PlayerEx::InstallHook();
			MaxsuPoise::PerkEntryHook::Install();
			MaxsuPoise::HUD::GetSingleton().Initialize();
		}
	}

}
