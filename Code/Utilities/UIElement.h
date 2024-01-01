#pragma once
#include "UltraEngine.h"

class ObjectBar;
namespace UltraEngine::Editor
{
    class Program;
}

namespace UltraEngine
{
    class UIElement : public Panel
    {
    protected:
        friend UltraEngine::Editor::Program;
        friend ObjectBar;
        static bool EventCallback(const Event& e, shared_ptr<Object> extra);
        virtual bool ProcessEvent(const Event& e);
        virtual void Listen(const EventId eventid = EVENT_NONE, shared_ptr<Object> widget = NULL);
    };
}

#ifdef LUA_VERSION
SOL_BASE_CLASSES(UltraEngine::UIElement, UltraEngine::Widget, UltraEngine::Object);
#endif