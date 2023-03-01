#pragma once
#include "UltraEngine.h"
#include "VectorEdit.h"

namespace UltraEngine
{
	enum ColorEditStyle
	{
		COLOREDIT_DEFAULT,
		COLOREDIT_RGB = COLOREDIT_DEFAULT,
		COLOREDIT_RGBA
	};

	class ColorEdit : public Widget
	{
		static bool EventCallback(const Event& e, shared_ptr<Object> extra);
		shared_ptr<Widget> vectoredit;
		shared_ptr<Widget> button, cover;
		friend shared_ptr<Widget> CreateColorEdit(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const ColorEditStyle style);
	public:
	};

	extern shared_ptr<Widget> CreateColorEdit(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const ColorEditStyle style = COLOREDIT_DEFAULT);
};