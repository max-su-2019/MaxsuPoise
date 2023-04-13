#pragma once

namespace MaxsuPoise
{
	class PerkEntryHook
	{
		PerkEntryHook() = delete;
		~PerkEntryHook() = delete;

	public:
		static void Install()
		{
			stl::write_thunk_call<ApplyPerkEntryPoint_Aggressor>(REL::RelocationID(36700, 37710).address() + REL::Relocate(0x9A, 0xA1, 0x9A));  // 1.5.97 1405FA1B0
			stl::write_thunk_call<ApplyPerkEntryPoint_Target>(REL::RelocationID(36700, 37710).address() + REL::Relocate(0xAE, 0xB9, 0xAE));     // 1.5.97 1405FA1B0
		}

	private:
		struct ApplyPerkEntryPoint_Aggressor
		{
			static void thunk(INT32 entry, RE::Actor* target, RE::Actor* aggressor, float& staggerMult)
			{
				return;
				func(entry, target, aggressor, staggerMult);
			}
			static inline REL::Relocation<decltype(thunk)> func;
		};

		struct ApplyPerkEntryPoint_Target
		{
			static void thunk(INT32 entry, RE::Actor* target, RE::Actor* aggressor, float& staggerMult)
			{
				return;
				func(entry, target, aggressor, staggerMult);
			}
			static inline REL::Relocation<decltype(thunk)> func;
		};
	};

}