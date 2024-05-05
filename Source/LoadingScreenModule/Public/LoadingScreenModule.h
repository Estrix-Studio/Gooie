#pragma once

#include "Modules/ModuleInterface.h"


class ILoadingScreenModule : public IModuleInterface
{
public:
	virtual void StartInGameLoadingScreen() = 0;

};
