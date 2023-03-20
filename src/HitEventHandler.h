#pragma once

namespace MaxsuPoise
{
	class HitEventHandler
	{
	public:
		static void InstallHooks()
		{
			Hooks::Install();
		}

	protected:
		struct Hooks
		{
			struct ProcessHitEvent
			{
				static void thunk(RE::Actor* target, RE::HitData* hitData);

				static inline REL::Relocation<decltype(thunk)> func;
			};

			static void Install()
			{
				stl::write_thunk_call<ProcessHitEvent>(REL::RelocationID(37673, 38627).address() + REL::Relocate(0x3C0, 0x4A8, 0x3C0));  // 1.5.97 140628C20
			}
		};

	private:
		constexpr HitEventHandler() noexcept = default;
		HitEventHandler(const HitEventHandler&) = delete;
		HitEventHandler(HitEventHandler&&) = delete;

		~HitEventHandler() = default;

		HitEventHandler& operator=(const HitEventHandler&) = delete;
		HitEventHandler& operator=(HitEventHandler&&) = delete;
	};
}