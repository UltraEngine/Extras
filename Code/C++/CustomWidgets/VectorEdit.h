#pragma once
#include "UltraEngine.h"
#include "Spinner.h"
#include "ColorEdit.h"

namespace UltraEngine
{
	enum VectorEditStyle
	{
		VECTOREDIT_2D = 1,
		VECTOREDIT_3D = 2,
		VECTOREDIT_4D = 4,
		VECTOREDIT_INTEGER = 8
	};
	inline VectorEditStyle operator|(VectorEditStyle a, VectorEditStyle b)
	{
		return static_cast<VectorEditStyle>(static_cast<int>(a) | static_cast<int>(b));
	};

	class ColorEdit;
	enum ColorEditStyle;

	class VectorEdit : public Widget
	{
		int elements;
		std::array< std::shared_ptr<Widget>, 4> spinner;
		std::array< std::shared_ptr<Widget>, 4> label;
		friend shared_ptr<Widget> CreateVectorEdit(const int, const int, const int, const int, shared_ptr<Widget>, const VectorEditStyle);
		virtual void UpdateLayout();
		static bool EventCallback(const Event& e, shared_ptr<Object> extra);
		friend shared_ptr<Widget> CreateColorEdit(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const ColorEditStyle style);
	public:
		virtual void SetRange(const double x, const double y);
		virtual void SetColor(const Vec4& color, const WidgetColor index = WIDGETCOLOR_BACKGROUND);
	};

	extern shared_ptr<Widget> CreateVectorEdit(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const VectorEditStyle style = VECTOREDIT_3D);
};