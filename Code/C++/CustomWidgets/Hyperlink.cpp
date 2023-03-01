#include "UltraEngine.h"
#include "Hyperlink.h"

namespace UltraEngine
{
	bool HyperlinkCallback(const Event& e, shared_ptr<Object> extra)
	{
		iVec2 position = e.position;
		auto widget = e.source->As<Widget>();
		widget->Lock();

		iVec2 size;
		size.x = widget->gui->GetTextWidth(NULL, 1, widget->text, 0);
		size.y = widget->gui->GetFontHeight(NULL, 0, 0);

		if ((widget->style & LABEL_RIGHT) != 0)
		{
			position.x -= widget->size.x - size.x;
		}
		else if ((widget->style & LABEL_CENTER) != 0)
		{
			position.x -= (widget->size.x - size.x) / 2;
		}

		switch (e.id)
		{
		case EVENT_MOUSEDOWN:
			//if (position.x >= 0 and position.y >= 0 and position.x < size.x and position.y < size.y)
		{
			if (e.data == MOUSE_LEFT) EmitEvent(EVENT_WIDGETACTION, widget);
		}

		break;
		//case EVENT_MOUSEMOVE:
		case EVENT_MOUSEENTER:
			if (position.x >= 0 and position.y >= 0 and position.x < size.x and position.y < size.y)
			{
				//widget->SetCursor(CURSOR_HAND);
				//widget->SetColor(0, 0.5, 1, 1, WIDGETCOLOR_FOREGROUND);
			}
			//
			//{
	//widget->SetCursor(CURSOR_DEFAULT);
			////	widget->SetColor(1, 1, 1, 1, WIDGETCOLOR_FOREGROUND);
			//}
			break;
		case EVENT_MOUSELEAVE:
			//widget->SetColor(1, 1, 1, 1, WIDGETCOLOR_FOREGROUND);
			//widget->SetCursor(CURSOR_DEFAULT);
			break;
		}
		if ((widget->style & LABEL_RIGHT) != 0)
		{
			widget->SetColor(widget->GetColor(WIDGETCOLOR_SELECTION), WIDGETCOLOR_FOREGROUND);
		}
		widget->Unlock();
		return true;
	}

	shared_ptr<Widget> CreateHyperlink(const WString& text, const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const LabelStyle style)
	{
		auto widget = CreateLabel(text, x, y, width, height, parent, style);
		ListenEvent(EVENT_NONE, widget, HyperlinkCallback);
		if ((widget->style & LABEL_RIGHT) != 0)
		{
			widget->SetColor(widget->GetColor(WIDGETCOLOR_SELECTION), WIDGETCOLOR_FOREGROUND);
		}
		iVec2 size;
		size.x = widget->gui->GetTextWidth(NULL, 1, widget->text, 0) + 2;
		size.y = widget->gui->GetFontHeight(NULL, 1, 0) + 2;
		widget->SetColor((widget->GetColor(WIDGETCOLOR_SELECTION) * 1.0 + widget->GetColor(WIDGETCOLOR_FOREGROUND) * 0.0), WIDGETCOLOR_FOREGROUND);
		if ((widget->style & LABEL_RIGHT) != 0)
		{
		//	widget->SetShape(x + width - size.x, widget->position.y, size.x, size.y);
		}
		else
		{
		//	widget->SetShape(x, y, size.x, size.y);
		}
		widget->SetCursor(CURSOR_HAND);
		return widget;
	}
}