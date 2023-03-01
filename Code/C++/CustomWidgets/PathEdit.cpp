#include "UltraEngine.h"
#include "PathEdit.h"

namespace UltraEngine
{
	PathEdit::PathEdit()
	{
		pattern = "All Files:*";
	}

	void PathEdit::SetText(const WString& text)
	{
		textfield->SetText(StripDir(text));
		this->m_text = text;
	}

	void PathEdit::SetFontScale(const float scale)
	{
		textfield->SetFontScale(scale);
		button->SetFontScale(scale);
	}

	void PathEdit::SetColor(const float r, const float g, const float b, const float a, const WidgetColor index, const int deltatime)
	{
		textfield->SetColor(r, g, b, a, index, deltatime);
		button->SetColor(r, g, b, a, index, deltatime);
	}

	void PathEdit::SetColor(const float r, const float g, const float b, const float a, const WidgetColor index)
	{
		textfield->SetColor(r,g,b,a, index);
		button->SetColor(r,g,b,a, index);
	}

	bool PathEdit::EventCallback(const Event& e, shared_ptr<Object> extra)
	{
		auto pathedit = extra->As<PathEdit>();
		if (e.id == EVENT_WIDGETACTION)
		{
			if (e.source == pathedit->button)
			{
				WString path, title;
				if ((PATHEDIT_FOLDER & pathedit->style) != 0)
				{
					if ((pathedit->style & PATHEDIT_SAVE) != 0) title = "Save Folder"; else title = "Open Folder";
					path = RequestDir(title, pathedit->text);
				}
				else
				{
					if ((pathedit->style & PATHEDIT_SAVE) != 0) title = "Save File"; else title = "Open File";
					path = RequestFile(title, pathedit->text, pathedit->pattern, 0, (pathedit->style & PATHEDIT_SAVE) != 0);
				}
				if (path != pathedit->text)
				{
					pathedit->SetText(path);
					EmitEvent(EVENT_WIDGETACTION, pathedit, 0, 0, 0, 0, 0, NULL, path);
				}
			}
		}
		return true;
	}

	void PathEdit::SetPattern(const WString& pattern)
	{
		this->pattern = pattern;
	}

    shared_ptr<PathEdit> CreatePathEdit(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const PathEditStyle style)
    {
        int sw = 1;
        auto pathedit = std::make_shared<PathEdit>();
		pathedit->Initialize("", x, y, width, height, parent, style);
		pathedit->textfield = CreateTextField(0, 0, width - sw+1 - height, height, pathedit, TEXTFIELD_READONLY);
		pathedit->textfield->SetLayout(1, 1, 1, 1);
		pathedit->button = CreateButton("...", pathedit->textfield->position.x + pathedit->textfield->size.x, pathedit->textfield->position.y, height, height, pathedit);
		//pathedit->button->SetRadius(0);
		pathedit->button->SetLayout(0, 1, 1, 0);
        ListenEvent(EVENT_WIDGETACTION, pathedit->button, PathEdit::EventCallback, pathedit);
        return pathedit;
    }
}