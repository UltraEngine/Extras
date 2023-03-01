#include "UltraEngine.h"
#include "VectorEdit.h"

namespace UltraEngine
{
	void VectorEdit::UpdateLayout()
	{
		auto temp = m_kids;
		m_kids.clear();// this prevents ugly resizing artifacts
		Widget::UpdateLayout();
		m_kids = temp;
		int pad = Round(4.0f * gui->scale);
		int lw = Round(12.0f * gui->scale);

		int ew = size.x / elements;
		int mw = Round(60.0f * gui->scale);
		if (ew < mw)
		{
			for (int n = 0; n < elements; ++n)
			{
				label[n]->SetHidden(true);
				lw = 0;
			}
		}
		for (int n = 0; n < elements; ++n)
		{
			spinner[n]->SetShape(lw + Round(float(n) * float(size.x) / float(elements)) + pad,0,-pad+Round(float(size.x)/float(elements))-lw,size.y);
			if (ew >= mw) label[n]->SetShape(spinner[n]->position.x - lw, 0, lw, size.y);
		}
		if (ew >= mw)
		{
			for (int n = 0; n < elements; ++n)
			{
				label[n]->SetHidden(false);
			}
		}
	}

	void VectorEdit::SetColor(const Vec4& color, const WidgetColor index)
	{
		for (int n = 0; n < elements; ++n)
		{
			spinner[n]->SetColor(color, index);
		}
		Widget::SetColor(color, index);
	}

	bool VectorEdit::EventCallback(const Event& e, shared_ptr<Object> extra)
	{
		int n;
		auto v = extra->As<VectorEdit>();
		switch (e.id)
		{
		case EVENT_WIDGETACTION:
			for (int n = 0; n < v->elements; ++n)
			{
				if (v->spinner[n] == e.source)
				{
					EmitEvent(EVENT_WIDGETACTION, v);
					return false;
				}
			}
			break;
		case EVENT_WIDGETGAINFOCUS:
			for (int n = 0; n < v->elements; ++n)
			{
				if (v->spinner[n] == e.source)
				{
					v->label[n]->SetColor(v->color[WIDGETCOLOR_SELECTION], WIDGETCOLOR_FOREGROUND);
					//v->spinner[n]->SelectText(0,-1);
					EmitEvent(EVENT_WIDGETGAINFOCUS, v);
					return false;
				}
			}
			break;
		case EVENT_WIDGETLOSEFOCUS:
			for (int n = 0; n < v->elements; ++n)
			{
				if (v->spinner[n] == e.source)
				{
					v->label[n]->SetColor(0.5, 0.5, 0.5, 1, WIDGETCOLOR_FOREGROUND);
					EmitEvent(EVENT_WIDGETLOSEFOCUS, v);
					return false;
				}
			}
			break;
		}
	}

	void VectorEdit::SetRange(const double x, const double y)
	{
		for (int n = 0; n < elements; ++n)
		{
			spinner[n]->SetRange(x, y);
		}
	}

	shared_ptr<Widget> CreateVectorEdit(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const VectorEditStyle style)
	{
		auto widget = std::make_shared<VectorEdit>();
		widget->Initialize(L"", x, y, width, height, parent, style);
		widget->elements = 3;
		if (style & VECTOREDIT_2D) widget->elements = 2;
		if (style & VECTOREDIT_4D) widget->elements = 4;
		std::array<WString, 4> tags = { "X","Y","Z","W" };
		for (int n = 0; n < widget->elements; ++n)
		{
			auto sflags = SPINNER_NOSTEPPER;
			if ((style & VECTOREDIT_INTEGER) == 0) sflags = sflags | SPINNER_FLOAT;
			widget->spinner[n] = CreateSpinner(0, 0, 100, height, widget, sflags);
			widget->label[n] = CreateLabel(tags[n],0, 0, 100, height, widget, LABEL_MIDDLE);
			widget->spinner[n]->SetColor(0.8,0.8,0.8,1, WIDGETCOLOR_FOREGROUND);
			widget->label[n]->SetColor(0.5, 0.5, 0.5, 1, WIDGETCOLOR_FOREGROUND);
			ListenEvent(EVENT_NONE, widget->spinner[n], VectorEdit::EventCallback, widget);
		}
		widget->UpdateLayout();
		return widget;
	}
};