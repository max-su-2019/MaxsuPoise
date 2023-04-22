#include "LoadGame.h"
#include "Hooks/HitEventHandler.h"
#include "Hooks/PerkEntry_Hooks.h"
#include "Hooks/PoiseRegenHandler.h"
#include "SettingsHandler.h"

namespace MaxsuPoise
{
	void EventCallback(SKSE::MessagingInterface::Message* msg)
	{
		if (msg->type == SKSE::MessagingInterface::kPostPostLoad) {
			MaxsuPoise::SettingsHandler::Register();
			MaxsuPoise::HitEventHandler::InstallHooks();
			MaxsuPoise::PoiseRegenHandler::CharacterEx::InstallHook();
			MaxsuPoise::PoiseRegenHandler::PlayerEx::InstallHook();
			MaxsuPoise::PerkEntryHook::Install();
		}
	}

}
