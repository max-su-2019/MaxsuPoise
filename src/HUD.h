#pragma once
#include "TrueHUDApi.h"

namespace MaxsuPoise
{
	class HUD
	{
	private:
		HUD(HUD&) = delete;
		HUD& operator=(HUD&&) = delete;
		void operator=(HUD&) = delete;
		HUD();

		static void FlashSpecialBar(RE::Actor* actor, float current, float total);

		static inline TRUEHUD_API::IVTrueHUD4* api = nullptr;
		static inline std::atomic<float> flashed = -1;

	public:
		[[nodiscard]] static HUD& GetSingleton()
		{
			static HUD instance;
			return instance;
		}

		void Initialize();
	};
}
