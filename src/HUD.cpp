#include "HUD.h"
#include "PoiseHealthHandler.h"
#include "SettingsHandler.h"
#include "Utils.h"

namespace MaxsuPoise
{
	HUD::HUD()
	{
		if (!GetGameSettingBool("bMaxsuPoise_EnableTrueHUD", true)) {
			WARN("TrueHUD integration disabled");
			return;
		}

		api = reinterpret_cast<TRUEHUD_API::IVTrueHUD4*>(TRUEHUD_API::RequestPluginAPI(TRUEHUD_API::InterfaceVersion::V4));
		if (!api) {
			WARN("TrueHUD not found");
		}
	}

	void HUD::FlashSpecialBar(RE::Actor* actor, float current, float total)
	{
		float barFlashThreshold = GetGameSettingFloat("fMaxsuPoise_BarFlashThreshold", 30.0f);

		if (actor && barFlashThreshold > 0) {
			bool belowThreshold = ((current / total) * 100.0) < barFlashThreshold;
			int flashType = 0;

			if (belowThreshold) {
				if (actor->IsPlayerRef()) {
					if (flashed < 0 || current < flashed) {
						flashType = 1;  // long
						flashed = current;
					} else {
						flashType = 0;
					}
				} else if (actor->AsActorState()->actorState2.staggered) {
					flashType = 2;  // short
				}

				if (flashType > 0) {
					api->FlashActorSpecialBar(SKSE::GetPluginHandle(), actor->GetHandle(), flashType == 1 ? true : false);
					if (actor->IsPlayerRef() && flashed > 0) {
						api->OverrideSpecialBarColor(actor->GetHandle(), TRUEHUD_API::BarColorType::FlashColor, 0xFF0000);
					}
				}
			} else if (actor->IsPlayerRef()) {
				if (flashed >= 0) {
					api->RevertSpecialBarColor(actor->GetHandle(), TRUEHUD_API::BarColorType::FlashColor);
					flashed = -1;
				}
			}
		}
	}

	void HUD::Initialize()
	{
		if (!api) {
			return;
		}

		auto getCurrentSpecial = [](RE::Actor* actor) -> float {
			float total = PoiseHealthHandler::GetTotalPoiseHealth(actor);
			float current = PoiseHealthHandler::GetCurrentPoiseHealth(actor);
			FlashSpecialBar(actor, current, total);
			return current;
		};

		if (api->RequestSpecialResourceBarsControl(SKSE::GetPluginHandle()) == TRUEHUD_API::APIResult::OK) {
			if (api->RegisterSpecialResourceFunctions(SKSE::GetPluginHandle(),
					getCurrentSpecial,
					PoiseHealthHandler::GetTotalPoiseHealth, true) != TRUEHUD_API::APIResult::OK) {
				WARN("Error registering TrueHUD callback functions");
				api = nullptr;
			}
		}
	}
}