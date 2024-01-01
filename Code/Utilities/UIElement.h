#pragma once
#include "UltraEngine.h"

namespace UltraEngine
{
    class UIElement : public Object
    {
    protected:
        static bool EventCallback(const Event& e, shared_ptr<Object> extra);
        virtual bool ProcessEvent(const Event& e);
        virtual void Listen(const EventId eventid = EVENT_NONE, shared_ptr<Object> widget = NULL);
    };
}

#ifdef LUA_VERSION
SOL_BASE_CLASSES(UltraEngine::UIElement, UltraEngine::Widget, UltraEngine::Object);
#endif

/* 
// Example usage

class MainPanel : public UIElement
{
public:
	shared_ptr<Widget> panel, button;

	bool Initialize(int x, int y, int width, int height, shared_ptr<Widget> parent)
	{
		panel = CreatePanel(x, y, width, height, parent);
		button = CreateButton("TEST", 20, 20, 80, 30, panel);
		Listen(EVENT_WIDGETACTION, button);
		return true;
	}
      
	bool ProcessEvent(const Event& e)
	{
		if (e.id == EVENT_WIDGETACTION and e.source == button)
		{
			Print("PRESS");
			return false;// stop processing this event
		}
		return true;// allow other hooks to process the event
	}
};
*/
