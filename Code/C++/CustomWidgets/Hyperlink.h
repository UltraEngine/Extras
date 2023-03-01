#pragma once
#include "UltraEngine.h"

namespace UltraEngine
{
	extern shared_ptr<Widget> CreateHyperlink(const WString& text, const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const LabelStyle style = LABEL_DEFAULT);
};