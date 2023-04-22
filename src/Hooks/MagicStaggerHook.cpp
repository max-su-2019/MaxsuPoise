#include "MagicStaggerHook.h"
#include "StaggerHandler.h"

namespace MaxsuPoise
{

	void MagicStaggerHook::Hooks::ProcessHitEvent::thunk(RE::Actor* a_target, float a_staggerMult, RE::Actor* a_aggressor)
	{
		StaggerHandler::ProcessMagicStagger(a_target, a_staggerMult, a_aggressor);
	}
}