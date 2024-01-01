#include "UltraEngine.h"
#include "UIElement.h"

namespace UltraEngine
{
    bool UIElement::EventCallback(const Event& e, shared_ptr<Object> extra)
    {
        auto elem = extra->As<UIElement>();
        return elem->ProcessEvent(e);
    }

    bool UIElement::ProcessEvent(const Event& e)
    {
        return true;
    }

    void UIElement::Listen(const EventId eventid, shared_ptr<Object> source)
    {
        ListenEvent(eventid, source, UIElement::EventCallback, Self());
    }
}