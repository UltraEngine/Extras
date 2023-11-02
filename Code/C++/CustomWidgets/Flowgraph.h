#include "UltraEngine.h"

namespace UltraEngine
{
	inline const EventId EVENT_WIDGETCONNECT = EventId(6500001);
	inline const EventId EVENT_WIDGETDISCONNECT = EventId(6500002);

	class FlowGraphSubNode;
	class FlowGraphMethod;

	class FlowGraphConnection : public Widget
	{
	public:
		std::weak_ptr<FlowGraphMethod> source;
		std::weak_ptr<FlowGraphMethod> dest;
		WString input;
		WString output;
		std::vector<iVec2> points;
		iVec2 start, end;
		
		virtual void MoveToTop();
		virtual void Draw(const int x, const int y, const int w, const int h);
		virtual void Free();

		friend shared_ptr<FlowGraphConnection> CreateFlowGraphConnection(const int x0, const int y0, const int x1, const int y1, shared_ptr<Widget> parent);
	};

	class FlowGraphMethod : public Object
	{
	public:
		WString name;
		int type;
		std::vector<shared_ptr<FlowGraphConnection> > GetInputs();
		shared_ptr<Widget> label, knob;
		std::set<shared_ptr<FlowGraphConnection> > outputs;
		std::set<std::weak_ptr<FlowGraphConnection>, std::owner_less<std::weak_ptr<FlowGraphConnection> > > inputs;
		virtual shared_ptr<FlowGraphConnection> Connect(shared_ptr<FlowGraphMethod> target);
	};

	class FlowGraphSubNode : public Object
	{
	public:
		WString name;
		shared_ptr<Widget> panel, label;
		std::vector<std::shared_ptr<FlowGraphMethod> > inputs;
		std::vector<std::shared_ptr<FlowGraphMethod> > outputs;
		std::vector<shared_ptr<FlowGraphConnection> > connections;

		virtual void Free();
		virtual int AddInput(const WString& name);
		virtual int AddOutput(const WString& name);
	};

	class FlowGraph;

	class FlowGraphNode : public Widget
	{
	public:
		int pickindex{ 0 };
		std::shared_ptr<Pixmap> background;
		std::weak_ptr<FlowGraph> graph;
		iVec2 mouseclickposition;
		bool mousepressed{ false };
		virtual void MouseDown(const MouseButton button, const int x, const int y);
		virtual void MouseUp(const MouseButton button, const int x, const int y);
		virtual void MouseMove(const int x, const int y);
		virtual void MouseLeave(const int x, const int y);
		virtual void Draw(const int x, const int y, const int width, const int height);
		std::vector<shared_ptr<FlowGraphSubNode> > subnodes;
		virtual void UpdateSize();
		virtual void MoveToTop();
		virtual void Free();
		virtual shared_ptr<FlowGraphSubNode> AddSubnode(const WString& name);
		friend shared_ptr<FlowGraphNode> CreateFlowGraphNode(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent);
	};

	class FlowGraph : public Widget
	{
	public:
		static bool EventHook(const Event& e, shared_ptr<Object> extra);
		shared_ptr<Menu> popupmenu, menu_remove;
		iVec2 offset, addedoffset;
		std::array<iVec2, 8> mousedownposition;
		std::array<bool, 8> mousepressed;
		std::weak_ptr<FlowGraphConnection> activeconnection;
		std::weak_ptr<FlowGraphMethod> hoveredmethod, hoveredtarget;
		inline static shared_ptr<Icon> knobicon[2];
		virtual void Draw(const int x, const int y, const int width, const int height);
		shared_ptr<Pixmap> background;
		virtual void MouseDown(const MouseButton button, const int x, const int y);
		virtual void MouseMove(const int x, const int y);
		virtual shared_ptr<FlowGraphMethod> GetMethodAtPoint(const int x, const int y, const int type);
		virtual void MouseUp(const MouseButton button, const int x, const int y);
		//virtual bool Initialize(const WString& s, const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const int style);
		friend shared_ptr<FlowGraph> CreateFlowGraph(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent);
	};

	extern shared_ptr<FlowGraph> CreateFlowGraph(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent);
	extern shared_ptr<FlowGraphNode> CreateFlowGraphNode(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent);
	extern shared_ptr<FlowGraphConnection> CreateFlowGraphConnection(const int x0, const int y0, const int x1, const int y1, shared_ptr<Widget> parent);
}