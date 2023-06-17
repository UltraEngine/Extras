#include "UltraEngine.h"

using namespace std;

namespace UltraEngine
{
	ComboBox::ComboBox()
	{
		//classname = "ComboBox";
		openstate = false;
		//color[WIDGETCOLOR_FOREGROUND] = Vec4(1, 1, 1, 1);
		//color[WIDGETCOLOR_BORDER] = Vec4(0, 0, 0, 1);
		//color[WIDGETCOLOR_HIGHLIGHT] = Vec4(51.f / 255.f, 151.f / 255.f, 1);
		//color[WIDGETCOLOR_POPUP] = Vec4(0.1, 0.1, 0.1, 1);
		//color[WIDGETCOLOR_BACKGROUND_BOTTOM] = Vec4(0.15, 0.15, 0.15, 1);
		//color[WIDGETCOLOR_SELECTION] = Vec4(0.4, 0.4, 0.4, 1);
		itemheight = 22;
		textindent = 4;
		//Revert();
	}

	void ComboBox::UpdateColors(const WidgetColor index)
	{
		Widget::UpdateColors(index);
		if (listbox) listbox->SetParent(NULL);
		listbox = NULL;
		dropdowninterface = NULL;
		dropdownwindow = NULL;
		Reset();
	}

	void ComboBox::RemoveItem(const int index)
	{
		if (listbox) listbox->RemoveItem(index);
		Widget::RemoveItem(index);
	}

	void ComboBox::Reset()
	{
		auto gui = GetInterface();
		blocks.clear();
		auto win = gui->window.lock();
		if (win != NULL)
		{
			auto flags = WINDOW_HIDDEN;
#ifdef __linux__
			flags |= WINDOW_RESIZABLE; 
#endif
			dropdownwindow = CreateWindow("", 0, 0, size.x, 100, win, flags);
			dropdowninterface = CreateInterface(dropdownwindow);
			dropdowninterface->colors = gui->colors;
			dropdowninterface->SetScale(gui->scale);
			auto sz = dropdownwindow->ClientSize();
			auto panel = CreatePanel(0, 0, sz.x, sz.y, dropdowninterface->root, PANEL_BORDER);
			panel->SetLayout(1,1,1,1);
			listbox = CreateListBox(-1, -1, sz.x - 1, sz.y, panel);
			listbox->color = this->color;
			listbox->fontscale = this->fontscale;
			listbox->fontweight = this->fontweight;
			listbox->SetLayout(1,1,1,1);
			listbox->color[WIDGETCOLOR_SUNKEN] = listbox->color[WIDGETCOLOR_POPUP];
			//listbox->color[WIDGETCOLOR_BACKGROUND] = color[WIDGETCOLOR_POPUP];
			std::dynamic_pointer_cast<ListBox>(listbox)->combobox = std::dynamic_pointer_cast<ComboBox>(Self());
			listbox->m_items = items;
		}
		else
		{
			dropdownpanel = CreatePanel(0, 0, size.x, 100, gui->root, PANEL_BORDER);
			auto sz = dropdownpanel->ClientSize();
			listbox = CreateListBox(-1, -1, sz.x - 1, sz.y, dropdownpanel);
			listbox->color = this->color;
			listbox->fontscale = this->fontscale;
			listbox->fontweight = this->fontweight;
			listbox->SetLayout(1, 1, 1, 1);
			listbox->color[WIDGETCOLOR_SUNKEN] = listbox->color[WIDGETCOLOR_POPUP];
			std::dynamic_pointer_cast<ListBox>(listbox)->combobox = std::dynamic_pointer_cast<ComboBox>(Self());
			listbox->m_items = items;
			dropdownpanel->SetHidden(true);
		}

		//Background
		AddBlock(iVec2(0, 0), size, color[WIDGETCOLOR_BACKGROUND], false, cornerradius);

		//Border
		AddBlock(iVec2(0, 0), size, color[WIDGETCOLOR_BORDER], true, cornerradius);

		//Text
		AddBlock(L"", iVec2(textindent, 0), size, color[WIDGETCOLOR_FOREGROUND], TEXT_LEFT + TEXT_MIDDLE);

		//Button
		auto res = String(Round(gui->GetScale() * 100));
		//auto pixmap = gui->icon_combobox_arrow;// LoadPixmap("Materials/UI/combobox_arrow.svg");
		//if (pixmap) 
		AddBlock(gui->icon_combobox_arrow, iVec2(size.x - (size.y + gui->icon_combobox_arrow->size.x) / 2, (size.y - gui->icon_combobox_arrow->size.y) / 2));// , color[WIDGETCOLOR_FOREGROUND]);
	}

	/*void Button::Revert()
	{
		color[WIDGETCOLOR_FOREGROUND] = Vec4(1, 1, 1, 1);
		color[WIDGETCOLOR_BORDER] = Vec4(0, 0, 0, 1);
		color[WIDGETCOLOR_SELECTION] = Vec4(51 / 255, 151 / 255, 1);
		color[WIDGETCOLOR_BACKGROUND_TOP] = Vec4(0.20, 0.20, 0.20, 1);
		color[WIDGETCOLOR_BACKGROUND_BOTTOM] = Vec4(0.15, 0.15, 0.15, 1);
		cornerradius = 8;
	}*/

	/*void Button::ApplyTheme(const nlohmann::json& j3)
	{
		if (!j3["theme"].is_object()) return;
		if (!j3["theme"]["button"].is_object()) return;
		if (j3["theme"]["button"]["color"].is_object())
		{
			if (j3["theme"]["button"]["color"]["foreground"].is_array())
			{
				if (j3["theme"]["button"]["color"]["foreground"].size() == 4)
				{
					color[WIDGETCOLOR_FOREGROUND].r = j3["theme"]["button"]["color"]["foreground"][0];
					color[WIDGETCOLOR_FOREGROUND].g = j3["theme"]["button"]["color"]["foreground"][1];
					color[WIDGETCOLOR_FOREGROUND].b = j3["theme"]["button"]["color"]["foreground"][2];
					color[WIDGETCOLOR_FOREGROUND].a = j3["theme"]["button"]["color"]["foreground"][3];
				}
			}
		}
	}*/

	void ComboBox::ClearItems()
	{
		Widget::ClearItems();
		listbox->ClearItems();
	}

	void ComboBox::SetHidden(const bool state)
	{
		if (openstate)
		{
			EmitEvent(EVENT_WIDGETCLOSE, Self());
			Close();
		}
		Widget::SetHidden(state);
	}

	void ComboBox::SetFontScale(const float scale)
	{
		listbox->SetFontScale(scale);
		Widget::SetFontScale(scale);
	}
	
	void ComboBox::SetItemText(const int item, const WString& text)
	{
		Widget::SetItemText(item, text);
		if (listbox) listbox->SetItemText(item, text);
	}

	void ComboBox::SetColor(const float r, const float g, const float b, const float a, const WidgetColor index)
	{
		listbox->SetColor(r,g,b,a,index);
		Widget::SetColor(r,g,b,a,index);
	}

	int ComboBox::AddItem(const WString& text, const bool selected)
	{
		int n = Widget::AddItem(text, selected);
		listbox->AddItem(text, selected);
		return n;
	}

	shared_ptr<Widget> CreateComboBox(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const ComboBoxStyle style)
	{
		auto widget = std::make_shared<ComboBox>();
		if (!std::dynamic_pointer_cast<Widget>(widget)->Initialize("", x, y, width, height, parent, style)) return NULL;
		widget->Invalidate();
		return widget;
	}

	/*shared_ptr<Widget> CreateComboBox(const int x, const int y, const int width, const int height, shared_ptr<Interface> ui, const ComboBoxStyle style)
	{
		auto widget = std::make_shared<ComboBox>();
		if (!std::dynamic_pointer_cast<Widget>(widget)->Initialize("", x, y, width, height, ui, style)) return NULL;
		widget->Invalidate();
		return widget;
	}*/

	void ComboBox::MouseWheel(const int delta, const int x, const int y)
	{
		if (openstate) return;
		if (items.size() < 2) return;
		int n = GetSelectedItem();
		int old = n;
		n += delta;
		n = Clamp(n, 0, items.size()-1);
		if (n != old)
		{
			if (scrollrange != iVec2(-1))
			{
				if (n < scrollrange.x or n >= scrollrange.x + scrollrange.y) return;
			}
			SelectItem(n);
			EmitEvent(EVENT_WIDGETSELECT,Self(),n);
		}
	}

	void ComboBox::FilterItems(const WString& filter)
	{
		Widget::FilterItems(filter);
		if (listbox) listbox->FilterItems(filter);
	}

	void ComboBox::MouseEnter(const int x, const int y)
	{
	//	hoverstate = true;
	//	Redraw();
	}

	void ComboBox::MouseLeave(const int x, const int y)
	{
	//	hoverstate = false;
	//	Redraw();
	}

	void ComboBox::MouseDown(const MouseButton button, const int x, const int y)
	{
		if (button == MOUSE_LEFT)
		{
			if (openstate)
			{
				EmitEvent(EVENT_WIDGETCLOSE, Self());
				Close();
				//openstate = false;
			}
			else
			{
				EmitEvent(EVENT_WIDGETOPEN, Self());
				Open();
				//openstate = true;
			}
		}
	}

	void ComboBox::MouseUp(const MouseButton button, const int x, const int y)
	{
	}

	void ComboBox::GainFocus()
	{
		Redraw();
	}

	void ComboBox::LoseFocus()
	{
		if (ActiveWindow() == this->dropdownwindow) return;
		if (openstate)
		{
			EmitEvent(EVENT_WIDGETCLOSE, Self());
			Close();
		}
	}

	bool ComboBox::KeyDown(const KeyCode key)
	{
		if (key == KEY_RIGHT or key == KEY_DOWN)
		{
			if (openstate)
			{
				listbox->KeyDown(key);
				return true;
			}
			//if ((style & COMBOBOX_NOSCROLL) != 0) return;
			int item = GetSelectedItem() + 1;
			if (item < items.size())
			{
				if (scrollrange != iVec2(-1))
				{
					if (item < scrollrange.x or item >= scrollrange.x + scrollrange.y) return true;
				}
				SelectItem(item);
				if (openstate) listbox->SelectItem(item);
				EmitEvent(EVENT_WIDGETSELECT, Self(), item);
			}
			return true;
		}
		else if (key == KEY_LEFT or key == KEY_UP)
		{
			if (openstate)
			{
				listbox->KeyDown(key);
				return true;
			}
			//if ((style & COMBOBOX_NOSCROLL) != 0) return;
			int item = GetSelectedItem() - 1;
			if (item > -1)
			{
				if (scrollrange != iVec2(-1))
				{
					if (item < scrollrange.x or item >= scrollrange.x + scrollrange.y) return true;
				}
				SelectItem(item);
				if (openstate) listbox->SelectItem(item);
				EmitEvent(EVENT_WIDGETSELECT, Self(), item);
			}
			return true;
		}
		else if (key == KEY_ESCAPE)
		{
			if (openstate)
			{
				EmitEvent(EVENT_WIDGETCLOSE, Self());
				Close();
			}
			return true;
		}
		else if (key == KEY_ENTER)
		{
			if (openstate)
			{
				int item = listbox->GetSelectedItem();
				if (item != GetSelectedItem())
				{
					SelectItem(item);
					EmitEvent(EVENT_WIDGETSELECT, Self(), item);
				}
				EmitEvent(EVENT_WIDGETCLOSE, Self());
				Close();
			}
			return true;
		}
		return Widget::KeyDown(key);
	}

	void ComboBox::WindowDragBegin()
	{
		if (openstate)
		{
			EmitEvent(EVENT_WIDGETCLOSE, Self());
			Close();
		}
	}

	/*void ComboBox::SetFont(shared_ptr<Font> font, const int size)
	{
		if (this->font == font and this->fontsize == fontsize) return;
		this->font = font;
		this->fontsize = size;
		UpdateLayout();
		Redraw();
		listbox->SetFont(font, size);
	}*/

	void ComboBox::Open()
	{
		auto gui = GetInterface();
		auto window = gui->GetWindow();
		bool invert = false;
		int winheight = std::dynamic_pointer_cast<ListBox>(listbox)->ItemHeight() * items.size() + 2;

		auto pos = GetPosition(true);
		pos.y += size.y - 1;
		int ww = size.x;

		for (int n = 0; n < items.size(); ++n)
		{
			ww = Max(ww, Round(12.0f * gui->scale) + gui->GetTextWidth(NULL, fontscale, items[n].text, fontweight));
		}

		auto win = gui->window.lock();
		if (win != NULL)
		{
			pos = TransformCoord(pos, win, NULL);
				
			auto display = GetDisplayAtCoord(pos.x,pos.y);
				
			if (display)
			{
				auto ca = display->ClientArea();
				if (pos.y + winheight > display->ClientArea().y + display->ClientArea().w)
				{
					winheight = Min(winheight, (display->ClientArea().w + display->ClientArea().y - pos.y) / itemheight * itemheight);
					auto client = display->ClientArea();
					if ((pos.y - winheight > display->ClientArea().y) or (pos.y - size.y / 2 > display->ClientArea().y + (display->ClientArea().w) / 2))
					{
						invert = true;
						winheight = std::dynamic_pointer_cast<ListBox>(listbox)->ItemHeight() * items.size();
						winheight = Min(winheight, (pos.y - size.y - display->ClientArea().y) / itemheight * itemheight);
					}
				}
			}
		}

		pos = GetPosition(true) + iVec2(0, size.y);

		if (dropdownwindow)
		{
			pos = TransformCoord(pos, window, NULL);

			if (invert)
			{
				dropdownwindow->SetShape(pos.x, pos.y - size.y - winheight + 1, ww, winheight);
			}
			else
			{				
				dropdownwindow->SetShape(pos.x, pos.y - 1, ww, winheight);
			}

			std::dynamic_pointer_cast<ListBox>(listbox)->slider->hoverstate = 0;
			std::dynamic_pointer_cast<ListBox>(listbox)->slider->knobgrabbed = false;
			
			auto sz = dropdownwindow->ClientSize();
			//sz.y = std::dynamic_pointer_cast<ListBox>(listbox)->itemheight * items.size() + 2;
			//listbox->SetShape(0, 0, sz.x, sz.y);
		//	listbox->items = items;
			listbox->SelectItem(GetSelectedItem());
			dropdownwindow->SetHidden(false);
            
			//int y = 1;
			//while (y < sz.y)
			//{
			//	dropdownwindow->SetSize(sz.x, y);
			//	dropdownwindow->Paint(0,0,sz.x,sz.y);
			//	y += 1;
			//}
			listbox->Activate();
		}
		else
		{
			if (pos.y + winheight > gui->size.y and (pos.y - size.y > gui->size.y - pos.y))
			{
				winheight = Min(winheight, pos.y - size.y);
				pos.y = pos.y - size.y - winheight;
			}
			else
			{
				winheight = Min(winheight, gui->size.y - pos.y);
			}
			dropdownpanel->SetShape(pos.x, pos.y, size.x, winheight);
			dropdownpanel->SetParent(NULL);
			dropdownpanel->SetParent(gui->background);
			dropdownpanel->SetHidden(false);
			listbox->Activate();
		}
		openstate = true;
		Redraw();
	}

	void ComboBox::Close()
	{
		auto gui = GetInterface();
		if (not openstate) return;
		openstate = false;
		bool makeactive = listbox == gui->GetFocus();
		if (dropdownwindow)
		{			
			dropdownwindow->SetHidden(true);
		}
		else if (dropdownpanel)
		{
			dropdownpanel->SetHidden(true);
		}
		if (makeactive) Activate();
		Redraw();
	}

	void ComboBox::UpdateColors()
	{
		auto gui = listbox->GetInterface();
		if (listbox)
		{
			gui->colors = gui->colors;
			listbox->color = gui->colors;
			listbox->color[WIDGETCOLOR_SUNKEN] = listbox->color[WIDGETCOLOR_POPUP];
			listbox->As<ListBox>()->slider->color = gui->colors;
		}
		Widget::UpdateColors();
	}

	void ComboBox::Draw(const int x, const int y, const int width, const int height)
	{
		auto gui = GetInterface();
		if (blocks.empty()) return;
		int item = GetSelectedItem();
		blocks[0].size = size;
		blocks[1].size = size;
		blocks[2].size.x = size.x - size.y - blocks[2].position.x;
		blocks[2].size.y = size.y;
		blocks[3].position.x = size.x - (size.y + gui->icon_combobox_arrow->size.x) / 2;
		if (item == -1)
		{
			blocks[2].hidden = true;
		}
		else
		{
			blocks[2].SetText(items[item].text);
			blocks[2].hidden = false;
			if (blocks[2].text == L"") blocks[2].hidden = true;
		}
		blocks[1].color = color[WIDGETCOLOR_BORDER];
		blocks[2].color = color[WIDGETCOLOR_FOREGROUND];
		if (openstate)
		{
			blocks[0].color.r = color[WIDGETCOLOR_BACKGROUND].r * 0.75;
			blocks[0].color.g = color[WIDGETCOLOR_BACKGROUND].g * 0.75;
			blocks[0].color.b = color[WIDGETCOLOR_BACKGROUND].b * 0.75;
			blocks[0].color.a = color[WIDGETCOLOR_BACKGROUND].a;
			blocks[0].gradient = 1.0f;
		}
		else
		{
			blocks[0].color = color[WIDGETCOLOR_BACKGROUND];
			blocks[0].gradient = 0.75f;
		}
		//blocks[blocks.size() - 1].color = color[WIDGETCOLOR_FOREGROUND];;
		blocks[blocks.size() - 1].SetIcon(gui, gui->icon_combobox_arrow, 0.75f, color[WIDGETCOLOR_FOREGROUND]);
	}
}
