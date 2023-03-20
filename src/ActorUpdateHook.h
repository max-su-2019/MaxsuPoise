#pragma once

namespace MaxsuPoise
{
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

	class PoiseRegenHandler
	{
	public:
		static float GetPoiseRegenRate()
		{
			return 15.f;
		}

	private:
		PoiseRegenHandler() = delete;
		~PoiseRegenHandler() = delete;
	};

}