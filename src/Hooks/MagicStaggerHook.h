#pragma once

namespace MaxsuPoise
{
	class MagicStaggerHook
	{
	public:
		static void InstallHooks()
		{
			Hooks::Install();
		}

	protected:
		struct Hooks
		{
			struct ProcessStagger
			{
				static void thunk(RE::Actor* a_target, float a_staggerMult, RE::Actor* a_aggressor);

				static inline REL::Relocation<decltype(thunk)> func;
			};

			static void Install()
			{
				stl::write_thunk_call<ProcessStagger>(REL::RelocationID(34188, 34982).address() + 0x6F);  // 1.5.97 140563460
			}
		};

	private:
		constexpr MagicStaggerHook() noexcept = default;
		MagicStaggerHook(const MagicStaggerHook&) = delete;
		MagicStaggerHook(MagicStaggerHook&&) = delete;

		~MagicStaggerHook() = default;

		MagicStaggerHook& operator=(const MagicStaggerHook&) = delete;
		MagicStaggerHook& operator=(MagicStaggerHook&&) = delete;
	};
}