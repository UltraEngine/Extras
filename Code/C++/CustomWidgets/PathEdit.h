#pragma once
#include "UltraEngine.h"
#include <limits>

namespace UltraEngine
{
    enum PathEditStyle
    {
        PATHEDIT_FOLDER = 0,
        PATHEDIT_FILE = 1,
        PATHEDIT_SAVE = 2,
        PATHEDIT_DEFAULT = PATHEDIT_FILE
    };
    inline PathEditStyle operator|(PathEditStyle a, PathEditStyle b)
    {
        return static_cast<PathEditStyle>(static_cast<int>(a) | static_cast<int>(b));
    };

    class PathEdit : public Widget
    {
        WString pattern;
        shared_ptr<Widget> textfield;
        shared_ptr<Widget> button;
        static bool EventCallback(const Event& e, shared_ptr<Object> extra);
    public:
        PathEdit();

        virtual void SetColor(const float r, const float g, const float b, const float a = 1.0f, const WidgetColor index = WIDGETCOLOR_BACKGROUND, const int deltatime = 0);
        virtual void SetFontScale(const float scale);
        virtual void SetText(const WString& text);
        virtual void SetColor(const float r, const float g, const float b, const float a, const WidgetColor index = WIDGETCOLOR_BACKGROUND);
        virtual void SetPattern(const WString& filepattern);

        friend shared_ptr<PathEdit> CreatePathEdit(const int, const int, const int, const int, shared_ptr<Widget>, const PathEditStyle);
    };

    extern shared_ptr<PathEdit> CreatePathEdit(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const PathEditStyle style = PATHEDIT_DEFAULT);
}