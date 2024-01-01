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
