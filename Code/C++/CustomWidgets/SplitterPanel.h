#pragma once
#include "UltraEngine.h"

namespace UltraEngine
{
	enum SplitterPanelStyle
	{
		SPLIT_DEFAULT,
		SPLIT_VERTICAL,
		SPLIT_HORIZONTAL,
		SPLIT_DRAGGABLE
	};
	class SplitterPanel : public Widget
	{
	public:
		bool moved;
		bool locked;
		bool grabbed;
		iVec2 offset;
		int deltapos;
		iVec2 grabposition;
		shared_ptr<Widget> handle;
		int maxsize;
		bool flipped;
		int minsize;

		virtual void UpdateLayout();

		std::array<shared_ptr<Widget>, 2> area;

		SplitterPanel();

		virtual void SetFlipped(const bool flip);
		virtual void SetStyle(const SplitterPanelStyle style);
		virtual void SetOffset(const int offset);
		virtual void SetOffset(const int x, const int y);
		virtual int GetOffset();
		virtual void SetMaxSize(const int maxsize);
		virtual void SetMinSize(const int minsize);
		virtual void SetPosition(const iVec2& offset);
		virtual void SetShape(const int x, const int y, const int width, const int height);
		virtual void SetLocked(const bool lock);
		virtual bool GetLocked();
		virtual WidgetState GetState();

		static bool EventCallback(const Event& e, shared_ptr<Object> extra);
		bool ProcessEvent(const Event& e);

		friend shared_ptr<SplitterPanel> CreateSplitterPanel(const int, const int, const int, const int, shared_ptr<Widget>, const SplitterPanelStyle);
	};

	extern shared_ptr<SplitterPanel> CreateSplitterPanel(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const SplitterPanelStyle style = SPLIT_DEFAULT);
}