#pragma once

namespace MaxsuPoise
{
	class PoiseRegenHandler
	{
	public:
		class CharacterEx : public RE::Character
		{
		public:
			static void InstallHook()
			{
				REL::Relocation<std::uintptr_t> CharacterVtbl{ VTABLE[0] };
				func = CharacterVtbl.write_vfunc(0x0AD, &Hook_Update);
				INFO("Hook Actor Update!");
			}

		private:
			void Hook_Update(float a_delta);

			static inline REL::Relocation<decltype(&RE::Character::Update)> func;
		};
		friend class CharacterEx;

		static float GetPoiseRegenRate();
		static float GetPoiseRegenDelayTimer(RE::Actor* a_target);
		static float SetPoiseRegenDelayTimer(RE::Actor* a_target, const float& a_in);
		static float GetMaxRegenDelayTime();

	private:
		static void UpdatePoiseValue(RE::Actor* a_target, float a_delta);

		PoiseRegenHandler() = delete;
		~PoiseRegenHandler() = delete;
	};

}