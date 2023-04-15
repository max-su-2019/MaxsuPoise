#pragma once
#include "DKUtil/Config.hpp"

namespace MaxsuPoise
{
	using namespace DKUtil::Alias;
	using WEAPON_TYPE = RE::WEAPON_TYPE;

	class SettingsHandler : public dku::model::Singleton<SettingsHandler>
	{
	public:
		friend dku::model::Singleton<SettingsHandler>;
		std::map<WEAPON_TYPE, float> WeapTypeMultMap;

	private:
		SettingsHandler();
	};

}