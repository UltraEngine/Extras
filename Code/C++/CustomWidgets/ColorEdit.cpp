#include "UltraEngine.h"
#include "ColorEdit.h"

namespace UltraEngine
{
	bool ColorEdit::EventCallback(const Event& e, shared_ptr<Object> extra)
	{
		auto c = extra->As<ColorEdit>();
		if (e.id == EVENT_MOUSEDOWN)
		{
			if (e.source == c->button)
			{
				c->button->SetColor(c->color[WIDGETCOLOR_SELECTION],WIDGETCOLOR_BORDER);
				c->cover->SetHidden(false);
			}
		}
		else if (e.id == EVENT_MOUSEUP)
		{
			if (e.source == c->button)
			{
				c->button->SetColor(c->vectoredit->GetColor(WIDGETCOLOR_BORDER), WIDGETCOLOR_BORDER);
				c->cover->SetHidden(true);
				if (e.position.x >= 0 and e.position.y >= 0 and e.position.x < c->button->size.x and e.position.y < c->button->size.y)
				{
					int r, g, b;
					r = Clamp(Round(c->vectoredit->kids[0]->GetValue() ), 0, 255);
					g = Clamp(Round(c->vectoredit->kids[2]->GetValue() ), 0, 255);
					b = Clamp(Round(c->vectoredit->kids[4]->GetValue() ), 0, 255);
					auto color = RequestColor(r,g,b);
					if (color != -1 and color != Rgba(r,g,b,255))
					{
						r = Red(color);
						g = Green(color);
						b = Blue(color);
						int a = 255;
						c->button->SetColor(float(r) / 255.0f, float(g) / 255.0f, float(b) / 255.0f);
						c->vectoredit->kids[0]->SetValue(r);
						c->vectoredit->kids[2]->SetValue(g);
						c->vectoredit->kids[4]->SetValue(b);
						if (c->vectoredit->kids.size() > 6) a = c->vectoredit->kids[6]->GetValue();
						EmitEvent(Event(EVENT_WIDGETACTION, c, Rgba(r, g, b, a)));
					}
				}
			}
		}
		else if (e.id == EVENT_WIDGETACTION)
		{
			if (e.source == c->vectoredit)
			{
				int r = c->vectoredit->kids[0]->GetValue();
				int g = c->vectoredit->kids[2]->GetValue();
				int b = c->vectoredit->kids[4]->GetValue();
				int a = 255;
				if (c->vectoredit->kids.size() > 6) a = c->vectoredit->kids[6]->GetValue();
				c->button->SetColor(r, g, b);
				auto rgba = Rgba(r, g, b, a);
				//a = Alpha(rgba);
				//int i;
				//memcpy(&i, &rgba, 4);
				EmitEvent(Event(EVENT_WIDGETACTION, c, rgba));
				return false;
			}
		}
		return true;
	}

	shared_ptr<Widget> CreateColorEdit(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const ColorEditStyle style)
	{
		auto widget = std::make_shared<ColorEdit>();
		widget->Initialize(L"", x, y, width, height, parent, style);
		widget->vectoredit = CreateVectorEdit(height+2, 0, widget->size.x - height - 2, widget->size.y, widget, VECTOREDIT_3D | VECTOREDIT_INTEGER);
		widget->vectoredit->SetLayout(1,1,1,1);
		int in = 2;
		widget->button = CreatePanel(in, in, height-2* in, height-2* in, widget, PANEL_BORDER);
		widget->button->SetColor(1, 1, 1);
		//widget->button->SetRadius(5);
		widget->cover = CreatePanel(0, 0, widget->button->size.x, widget->button->size.y, widget->button);
		widget->cover->SetColor(widget->color[WIDGETCOLOR_SELECTION]);
		auto color = widget->GetColor(WIDGETCOLOR_BACKGROUND);
		color.a = 0.5f;
		widget->cover->SetColor(color, WIDGETCOLOR_BACKGROUND);
		widget->cover->SetInteractive(false);
		widget->cover->SetHidden(true);
		widget->vectoredit->As<VectorEdit>()->label[0]->SetText("R");
		widget->vectoredit->As<VectorEdit>()->label[1]->SetText("G");
		widget->vectoredit->As<VectorEdit>()->label[2]->SetText("B");
		widget->vectoredit->As<VectorEdit>()->spinner[0]->SetValue(255);
		widget->vectoredit->As<VectorEdit>()->spinner[1]->SetValue(255);
		widget->vectoredit->As<VectorEdit>()->spinner[2]->SetValue(255);
		widget->button->SetLayout(1, 0, 1, 1);
		widget->vectoredit->SetRange(0, 255);
	//	widget->button->SetCursor(CURSOR_HAND);
		ListenEvent(EVENT_NONE, widget->button, ColorEdit::EventCallback, widget);
		ListenEvent(EVENT_WIDGETACTION, widget->vectoredit, ColorEdit::EventCallback, widget);
		//widget->button->SetInteractive(false);
		return widget;
	}
}