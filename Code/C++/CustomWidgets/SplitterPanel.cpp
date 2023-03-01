#include "UltraEngine.h"
#include "SplitterPanel.h"

namespace UltraEngine
{
	SplitterPanel::SplitterPanel()
	{
		moved = false;
		locked = false;
		minsize = 0;
		maxsize = 0;
		offset = -1;
		grabbed = false;
		flipped = false;
	}

	shared_ptr<SplitterPanel> CreateSplitterPanel(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const SplitterPanelStyle style)
	{
		const int spacing = 2;

		auto widget = std::make_shared<SplitterPanel>();
		widget->Initialize("", x, y, width, height, parent, style);
		widget->area[0] = CreatePanel(0, 0, widget->size.x, widget->size.y / 2 - spacing, widget);
		widget->area[1] = CreatePanel(0, widget->size.y / 2 - spacing, widget->size.x, widget->size.y / 2 - spacing, widget);
		widget->handle = CreatePanel(0, widget->size.y / 2 - spacing, widget->size.x, widget->size.y / 2 - spacing, widget);
		widget->area[0]->SetColor(0, 0, 0, 0);
		widget->area[0]->SetColor(0, 0, 0, 0);
		widget->handle->SetColor(0, 0, 0, 0);
		widget->gui = parent->gui;
		if ((style & SPLIT_VERTICAL) != 0)
		{
			widget->handle->SetCursor(CURSOR_SIZEWE);
		}
		else
		{
			widget->handle->SetCursor(CURSOR_SIZENS);
		}
		widget->offset.x = -1;// widget->size.x / 2;
		widget->offset.y = -1;// widget->size.y / 2;
		ListenEvent(EVENT_NONE, widget->handle, SplitterPanel::EventCallback, widget);
		widget->UpdateLayout();
		return widget;
	}

	void SplitterPanel::SetPosition(const iVec2& offset)
	{
		this->offset = offset;
		UpdateLayout();
	}

	void SplitterPanel::SetLocked(const bool lock)
	{
		if (lock == locked) return;
		locked = lock;
		handle->SetHidden(locked);
	}

	bool SplitterPanel::GetLocked()
	{
		return locked;
	}

	void SplitterPanel::SetShape(const int x, const int y, const int width, const int height)
	{
		Widget::SetShape(x, y, width, height);
		UpdateLayout();
		area[0]->UpdateLayout();
		area[1]->UpdateLayout();
	}

	int SplitterPanel::GetOffset()
	{
		if ((style & SPLIT_VERTICAL) != 0)
		{
			return this->offset.x;
		}
		else
		{
			return this->offset.y;
		}
	}

	void SplitterPanel::SetOffset(const int offset)
	{
		if ((style & SPLIT_VERTICAL) != 0)
		{
			this->offset.x = Max(offset, Round(minsize * gui->scale));
		}
		else
		{
			this->offset.y = Max(offset, Round(minsize * gui->scale));
		}
		UpdateLayout();
	}

	void SplitterPanel::SetOffset(const int x, const int y)
	{
		this->offset.x = Max(x, Round(minsize * gui->scale));
		this->offset.y = Max(y, Round(minsize * gui->scale));
		UpdateLayout();
	}

	void SplitterPanel::SetFlipped(const bool flip)
	{
		if (flipped == flip) return;
		flipped = flip;
		UpdateLayout();
	}

	void SplitterPanel::SetMaxSize(const int maxsize)
	{
		this->maxsize = maxsize;
	}

	void SplitterPanel::SetMinSize(const int minsize)
	{
		this->minsize = minsize;
		if (offset.x != -1) offset.x = Max(offset.x, Round(minsize * gui->scale));
		if (offset.y != -1) offset.y = Max(offset.y, Round(minsize * gui->scale));
		UpdateLayout();
	}

	void SplitterPanel::UpdateLayout() 
	{
		auto sz = size;
		auto kids = m_kids;
		m_kids.clear();
		Widget::UpdateLayout();
		m_kids = kids;

		if ((style & SPLIT_VERTICAL) != 0)
		{
			offset.x = Min(offset.x, size.x / 2);
		}
		else
		{
			offset.y = Min(offset.y, size.y / 2);
		}

		if (maxsize != 0)
		{
			if (size.x > maxsize)
			{
				if (style != SPLIT_VERTICAL)
				{
					m_style = SPLIT_VERTICAL;
					if (offset.x == 0) offset.x = size.x / 2;
					offset.x = Min(offset.x, size.x / 2);
					offset.x = Max(offset.x, Round(minsize * gui->scale));
					handle->SetCursor(CURSOR_SIZEWE);
				}
			}
			else
			{
				if (style != SPLIT_HORIZONTAL)
				{
					m_style = SPLIT_HORIZONTAL;
					if (offset.y == 0) offset.y = size.y / 2;
					offset.y = Min(offset.y, size.y / 2);
					offset.y = Max(offset.y, Round(minsize * gui->scale));
					handle->SetCursor(CURSOR_SIZENS);
				}
			}
		}

		if ((style & SPLIT_VERTICAL) != 0)
		{
			if (offset.x == -1) offset.x = size.x / 2;
		}
		else
		{
			if (offset.y == -1) offset.y = size.y / 2;
		}
		auto offset = this->offset;
		if (flipped) offset = size - offset;
		const int spacing = Round(3.0f * gui->scale);
		if ((style & SPLIT_VERTICAL) != 0)
		{
			area[0]->SetShape(0, 0, size.x - offset.x - spacing, size.y);
			handle->SetShape(size.x - offset.x - spacing, 0, spacing * 2, size.y);
			area[1]->SetShape(size.x - offset.x + spacing, 0, offset.x- spacing * 2, size.y);
		}
		else
		{
			area[0]->SetShape(0, 0, size.x, size.y - offset.y - spacing);
			handle->SetShape(0, size.y - offset.y - spacing, size.x, spacing * 2);
			area[1]->SetShape(0, size.y - offset.y + spacing, size.x, offset.y - spacing * 2);
		}

		//if (size != sz)
		//{
		//	for (auto child : kids)
		//	{
		//		child->UpdateLayout();
		//	}
		//}
	}

	bool SplitterPanel::EventCallback(const Event& e, shared_ptr<Object> extra)
	{
		auto elem = extra->As<SplitterPanel>();
		return elem->ProcessEvent(e);
	}

	void SplitterPanel::SetStyle(const SplitterPanelStyle style)
	{
		if (this->style == style) return;
		this->m_style = style;
		UpdateLayout();
	}

	bool SplitterPanel::ProcessEvent(const Event& e)
	{
		switch (e.id)
		{
		case EVENT_MOUSEDOWN:
			if (e.source == handle)
			{
				if (e.data == MOUSE_LEFT)
				{
					grabbed = true;
					grabposition = e.position;
				}
				return false;
			}
			break;
		case EVENT_MOUSEUP:
			if (e.source == handle)
			{
				if (e.data == MOUSE_LEFT)
				{
					grabbed = false;
					if (moved)
					{
						if ((style & SPLIT_VERTICAL) != 0)
						{
							EmitEvent(EVENT_WINDOWDRAGEND, Self(), 0, offset.x);
						}
						else
						{
							EmitEvent(EVENT_WINDOWDRAGEND, Self(), 0, 0, offset.y);
						}
					}
					moved = false;
				}
				return false;
			}
			break;
		case EVENT_MOUSEMOVE:
			if (e.source == handle)
			{
				if (grabbed == true)
				{
					iVec2 coord = e.position + handle->position;
					int d = 0;
					if ((style & SPLIT_VERTICAL) != 0)
					{
						if (flipped)
						{
							offset.x = coord.x - grabposition.x;
						}
						else
						{
							offset.x = size.x - coord.x + grabposition.x;
						}
						offset.x = Max(Round(minsize * gui->scale), offset.x);
						offset.x = Min(offset.x, size.x / 2);
					}
					else
					{
						if (flipped)
						{
							offset.y = coord.y - grabposition.y;
						}
						else
						{
							offset.y = size.y - coord.y + grabposition.y;
						}
						offset.y = Max(Round(minsize * gui->scale), offset.y);
						offset.y = Min(offset.y, size.y / 2);
					}
					UpdateLayout();
					if (moved == false)
					{
						if ((style & SPLIT_VERTICAL) != 0)
						{
							EmitEvent(EVENT_WINDOWDRAGBEGIN, Self(), 1, offset.x);
						}
						else
						{
							EmitEvent(EVENT_WINDOWDRAGBEGIN, Self(), 1, 0, offset.y);
						}
					}
					if ((style & SPLIT_VERTICAL) != 0)
					{
						EmitEvent(EVENT_WINDOWMOVE, Self(), 1, offset.x);
					}
					else
					{
						EmitEvent(EVENT_WINDOWMOVE, Self(), 1, 0, offset.y);
					}
					moved = true;
				}
				return false;
			}
			break;
		}
		return true;
	}

	WidgetState SplitterPanel::GetState()
	{
		if (grabbed) return WIDGETSTATE_SELECTED;
		return WIDGETSTATE_UNSELECTED;
	}
}