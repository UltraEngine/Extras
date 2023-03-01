#pragma once
#include "UltraEngine.h"

namespace UltraEngine
{
	class ButtonStrip : public Widget
	{
		bool pressed;
		int hovereditem, presseditem;
	protected:
		virtual void MouseMove(const int x, const int y);
		virtual void MouseEnter(const int x, const int y);
		virtual void MouseLeave(const int x, const int y);
		virtual void MouseDown(const MouseButton button, const int x, const int y);
		virtual void MouseUp(const MouseButton button, const int x, const int y);
		virtual void Draw(const int x, const int y, const int width, const int height);
		virtual void Init(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent);
	public:
		ButtonStrip();

		friend shared_ptr<Widget> CreateButtonStrip(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent);
	};

	shared_ptr<Widget> CreateButtonStrip(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent);
};