#pragma once
#include "UltraEngine.h"

namespace UltraEngine
{
	class Widget;
	class Window;
	class Event;
	class Mouse;
	class iVec2;

	/**/

	class ComboBox : public Widget
	{
		bool openstate;
		int itemheight, textindent;
		bool hoverstate, pushstate;
		shared_ptr<Window> dropdownwindow;
		shared_ptr<Interface> dropdowninterface;
		shared_ptr<Widget> listbox;
		shared_ptr<Widget> dropdownpanel;
	protected:

		virtual void Reset();
		virtual void Open();
		virtual void Close();
		virtual void UpdateColors();
		virtual void UpdateColors(const WidgetColor index = WidgetColor(-1));

	public:
		ComboBox();

		virtual void SetColor(const float r, const float g, const float b, const float a, const WidgetColor index);
		virtual void SetFontScale(const float scale);
		//virtual void SetFont(shared_ptr<Font> font, const int size);
		virtual void FilterItems(const WString& filter);
		virtual void ClearItems();
		virtual int AddItem(const WString& text, const bool selected = false);
		virtual void MouseWheel(const int delta, const int x, const int y);
		virtual void GainFocus();
		virtual bool KeyDown(const KeyCode key);
		virtual void MouseEnter(const int x, const int y);
		virtual void MouseLeave(const int x, const int y);
		virtual void MouseDown(const MouseButton button, const int x, const int y);
		virtual void MouseUp(const MouseButton button, const int x, const int y);
		virtual void LoseFocus();
		virtual void Draw(const int x, const int y, const int width, const int height);
		virtual void WindowDragBegin();
		virtual void RemoveItem(const int index);
		virtual void SetItemText(const int item, const WString& text);
		virtual void SetHidden(const bool state);

		friend shared_ptr<Widget> CreateComboBox(const int, const int, const int, const int, shared_ptr<Widget>, const ComboBoxStyle);
		friend Widget;
		friend ListBox;
	};

	extern shared_ptr<Widget> CreateComboBox(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const ComboBoxStyle style = COMBOBOX_DEFAULT);
}
#ifdef LUA_VERSION
SOL_BASE_CLASSES(UltraEngine::ComboBox, UltraEngine::Widget, UltraEngine::Object);
#endif