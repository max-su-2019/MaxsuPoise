#include "HitEventHandler.h"
#include "StaggerHandler.h"

namespace MaxsuPoise
{
	void HitEventHandler::Hooks::ProcessHitEvent::thunk(RE::Actor* target, RE::HitData* hitData)
	{
		if (!target->IsPlayerRef()) {
			StaggerHandler::ProcessStagger(hitData);
			hitData->stagger = static_cast<uint32_t>(0.00);
		}

		func(target, hitData);
	}
}