#include "UltraEngine.h"
#include "Flowgraph.h"
#include "bezier.h"
 
namespace UltraEngine
{
	/*bool FlowGraph::Initialize(const WString& s, const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const int style)
	{
		Widget::Initialize(s, x, y, width, height, parent, style);
	}*/

	void FlowGraph::Draw(const int x, const int y, const int width, const int height)
	{
		iVec2 off = offset + addedoffset;

		auto c = activeconnection.lock();
		if (c)
		{
			auto s = c->source.lock();
			if (s)
			{
				s->label->SetColor(0, 1, 1, 1, WIDGETCOLOR_FOREGROUND);
				s->knob->SetIcon(knobicon[1], PIXMAP_CENTER, 0.5f);
			}
		}

		Vec4 minor = Vec4(0.1875, 0.1875, 0.1875, 1);
		Vec4 major = Vec4(0.25, 0.25, 0.25, 1);
		Vec4 axis = Vec4(0.375, 0.375, 0.375, 1);

		if (background == NULL)
		{
			Vec4 color;
			background = CreatePixmap(256, 256);
			background->Fill(Rgba(0,0,0,0));
			for (int x = 0; x < background->size.x; x += 8)
			{
				for (int y = 0; y < background->size.y; ++y)
				{
					if (x % 64 == 0)
					{
						color = major;
					}
					else
					{
						color = minor;
					}
					background->WritePixel(x, y, Rgba(color.r * 255.0f, color.g * 255.0f, color.b * 255.0f, 255));
				}
			}
			for (int x = 0; x < background->size.x; ++x)
			{
				for (int y = 0; y < background->size.y; y += 8)
				{
					if (y % 64 == 0)
					{
						color = major;
					}
					else
					{
						color = minor;
					}
					background->WritePixel(x, y, Rgba(color.r * 255.0f, color.g * 255.0f, color.b * 255.0f, 255));
				}
			}
		}

		blocks.clear();
		AddBlock(0, size, color[WIDGETCOLOR_SUNKEN], false);
		
		iVec2 bo;
		bo.x = (off.x % background->size.x);
		bo.y = (off.y % background->size.y);
		for (int x = -background->size.x; x < size.x + background->size.x; x += background->size.x)
		{
			for (int y = -background->size.y; y < size.y + background->size.y; y += background->size.y)
			{
				AddBlock(background, iVec2(x + bo.x, y + bo.y));
			}
		}

		//Border
		AddBlock(0, size, color[WIDGETCOLOR_BORDER], true);

		//AddBlock({iVec2(0,0), iVec2(1000,1000) }, Vec4(0,0,0,1));
		//blocks[blocks.size() - 1].linethickness = 4;

		//AddBlock({ iVec2(0,0), iVec2(1000,1000) });
		//blocks[blocks.size() - 1].linethickness = 2;
	} 

	shared_ptr<FlowGraph> CreateFlowGraph(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent)
	{
		auto fg = std::make_shared<FlowGraph>();
		fg->Initialize("", x, y, width, height, parent, 0);
		return fg;
	}

	shared_ptr<FlowGraphNode> CreateFlowGraphNode(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent)
	{
		auto ui = parent->GetInterface();
		auto node = std::make_shared<FlowGraphNode>();
		node->Initialize("", x, y, width, height, parent, PANEL_BORDER);
		node->cornerradius = 4.0f * ui->scale;



		//node->SetColor(node->GetColor(WIDGETCOLOR_SUNKEN));

		return node;
	}

	int FlowGraphSubNode::AddInput(const WString& name)
	{
		int y = 20 + 4 + inputs.size() * 20;
		auto m = std::make_shared<FlowGraphMethod>();
		m->name = name;
		m->label = CreateLabel(name, 20, y, 300, 20, panel, LABEL_TRUNCATE);
		m->label->SetFontScale(0.9);
		m->label->SetColor(0.9, WIDGETCOLOR_FOREGROUND);
		m->label->SetFontBold(true);
		inputs.push_back(m);
		m->knob = CreatePanel(0, y, 20, 16, panel);
		m->knob->SetColor(1, 1, 1, 0);
		m->type = 1;
		auto ui = panel->GetInterface();

		auto buffer = CreateBuffer();
		auto stream = CreateBufferStream(buffer);
		std::string s = "<?xml version=\"1.0\" encoding=\"utf-8\"?> <!-- Generator: Adobe Illustrator 16.0.3, SVG Export Plug-In . SVG Version: 6.00 Build 0) --> <!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"> <svg version=\"1.1\" id=\"Layer_2\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\" y=\"0px\" width=\"10px\" height=\"13px\" viewBox=\"0 0 10 13\" enable-root=\"new 0 0 10 13\" xml:space=\"preserve\"> <polygon fill=\"#FFFFFF\" points=\"1.922,12.5 8.078,6.501 1.922,0.5 \"/> </svg> ";
		buffer->Resize(s.length() + 1);
		buffer->PokeString(0, s);
		FlowGraph::knobicon[0] = LoadIcon(stream);
		s = "<?xml version=\"1.0\" encoding=\"utf-8\"?> <!-- Generator: Adobe Illustrator 16.0.3, SVG Export Plug-In . SVG Version: 6.00 Build 0) --> <!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"> <svg version=\"1.1\" id=\"Layer_2\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\" y=\"0px\" width=\"10px\" height=\"13px\" viewBox=\"0 0 10 13\" enable-root=\"new 0 0 10 13\" xml:space=\"preserve\"> <polygon fill=\"#00FFFF\" points=\"1.922,12.5 8.078,6.501 1.922,0.5 \"/> </svg> ";
		buffer->Resize(s.length() + 1);
		buffer->PokeString(0, s);
		FlowGraph::knobicon[1] = LoadIcon(stream);

		m->knob->SetIcon(FlowGraph::knobicon[0], PIXMAP_CENTER, 0.5);
		return inputs.size() - 1;
	}

	int FlowGraphSubNode::AddOutput(const WString& name)
	{
		int y = 20 + 4 + outputs.size() * 20;
		auto m = std::make_shared<FlowGraphMethod>();
		m->name = name;
		m->label = CreateLabel(name, panel->size.x / 2, y, panel->size.x / 2 - 20, 20, panel, LABEL_TRUNCATE | LABEL_RIGHT);
		m->label->SetFontScale(0.9);
		m->label->SetColor(0.9, WIDGETCOLOR_FOREGROUND);
		m->label->SetFontBold(true);
		outputs.push_back(m);
		m->type = 2;
		m->knob = CreatePanel(panel->size.x - 20, y, 20, 16, panel);
		m->knob->SetColor(1, 1, 1, 0);
		auto ui = panel->GetInterface();

		auto buffer = CreateBuffer();
		auto stream = CreateBufferStream(buffer);
		std::string s = "<?xml version=\"1.0\" encoding=\"utf-8\"?> <!-- Generator: Adobe Illustrator 16.0.3, SVG Export Plug-In . SVG Version: 6.00 Build 0) --> <!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"> <svg version=\"1.1\" id=\"Layer_2\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\" y=\"0px\" width=\"10px\" height=\"13px\" viewBox=\"0 0 10 13\" enable-root=\"new 0 0 10 13\" xml:space=\"preserve\"> <polygon fill=\"#FFFFFF\" points=\"1.922,12.5 8.078,6.501 1.922,0.5 \"/> </svg> ";
		buffer->Resize(s.length() + 1);
		buffer->PokeString(0, s);
		auto icon = LoadIcon(stream);

		m->knob->SetIcon(icon, PIXMAP_CENTER, 0.5);
		return inputs.size() - 1;
	}

	shared_ptr<FlowGraphSubNode> FlowGraphNode::AddSubnode(const WString& name)
	{
		auto ui = GetInterface();
		auto scale = ui->scale;
		int h = 100;
		int titleheight = 18.0f * scale;
		int y = titleheight;
		auto subnode = std::make_shared<FlowGraphSubNode>();
		for (auto child : subnodes)
		{
			y = child->panel->position.y + child->panel->size.y;
		}
		subnode->name = name;
		subnode->panel = CreatePanel(0, y, size.x, h, As<Widget>());
		subnode->panel->SetText(name);
		subnode->panel->SetColor(0, 0, 0, 0, WIDGETCOLOR_FOREGROUND);
		subnode->panel->SetInteractive(false);
		subnode->panel->SetColor(GetColor());
		subnode->label = CreateLabel(name, 0, 0, size.x, titleheight, subnode->panel, LABEL_MIDDLE | LABEL_CENTER);
		subnode->label->SetFontScale(0.9);
		subnode->label->SetFontBold(true);
		subnode->panel->SetColor(0, 0, 0, 0);
		subnodes.push_back(subnode);
		UpdateSize();
		return subnode;
	}

	void FlowGraphNode::UpdateSize()
	{
		auto ui = GetInterface();
		auto scale = ui->scale;
		int titleheight = 18.0f * scale;
		int h = titleheight;
		for (auto& subnode : subnodes)
		{
			int y = 0;
			for (auto child : subnode->panel->kids)
			{
				y = Max(y, child->position.y + child->size.y);
			}
			subnode->panel->SetShape(0, h, subnode->panel->size.x, y);
			h = subnode->panel->position.y + subnode->panel->size.y;// titleheight;
			//h += Max(subnode->inputs.size(), subnode->outputs.size()) * titleheight;
			//h += Round(8.0f * scale);
		}
		h += Round(4.0f * scale);
		SetShape(position.x, position.y, size.x, h);
	}

	shared_ptr<FlowGraphMethod> FlowGraph::GetMethodAtPoint(const int x, const int y, const int type)
	{
		auto ui = GetInterface();
		float scale = ui->scale;
		auto coord = GetPosition(true);
		coord.x += x;
		coord.y += y;
		for (auto child : kids)
		{
			auto node = child->As<FlowGraphNode>();
			if (not node) continue;
			if (coord.x >= node->position.x and coord.x < node->position.x + node->size.x)
			{
				if (coord.y >= node->position.y and coord.y < node->position.y + node->size.y)
				{
					for (auto subnode : node->subnodes)
					{
						auto subcoord = -subnode->panel->GetPosition(true);
						subcoord.x += x;
						subcoord.y += y;
						if (1 & type)
						{
							for (auto it = subnode->inputs.rbegin(); it != subnode->inputs.rend(); ++it)
							{
								auto m = *it;
								if (subcoord.x >= m->knob->position.x and subcoord.y >= m->knob->position.y)
								{
									if (subcoord.x < m->knob->position.x + m->knob->size.x and subcoord.y < m->knob->position.y + m->knob->size.y)
									{
										return m;
									}
								}
							}
						}
						if (2 & type)
						{
							for (auto it = subnode->outputs.rbegin(); it != subnode->outputs.rend(); ++it)
							{
								auto m = *it;
								if (subcoord.x >= m->knob->position.x and subcoord.y >= m->knob->position.y)
								{
									if (subcoord.x < m->knob->position.x + m->knob->size.x and subcoord.y < m->knob->position.y + m->knob->size.y)
									{
										return m;
									}
								}
							}
						}
					}
					break;
				}
			}
		}
		return nullptr;
	}

	void FlowGraph::MouseUp(const MouseButton button, const int x, const int y)
	{
		if (button == MOUSE_MIDDLE)
		{
			offset += addedoffset;
			addedoffset = 0;
		}
		mousepressed[button] = false;
		if (button == MOUSE_LEFT)
		{
			auto c = activeconnection.lock();
			if (c)
			{
				activeconnection.reset();
				auto s = c->source.lock();
				if (s)
				{
					s->label->SetColor(GetColor(WIDGETCOLOR_FOREGROUND), WIDGETCOLOR_FOREGROUND);
					s->knob->SetIcon(knobicon[0], PIXMAP_CENTER, 0.5f);
				}
				auto ui = GetInterface();
				float scale = ui->scale;
				auto coord = GetPosition(true);
				coord.x += x;
				coord.y += y;
				auto m = hoveredtarget.lock();// GetMethodAtPoint(x, y, 1);
				hoveredtarget.reset();
				if (m)
				{
					auto coord = m->knob->GetPosition(true);
					c->end.x = float(m->knob->GetParent()->GetPosition(true).x);// / scale;
					c->end.y = float(coord.y + m->knob->size.y / 2);// / scale;
					auto prevtarget = c->dest.lock();
					if (prevtarget)
					{
						auto it = prevtarget->inputs.find(c);
						if (it != prevtarget->inputs.end()) prevtarget->inputs.erase(it);
					}
					c->dest = m;
					c->SetColor(1,0.85,0,1, WIDGETCOLOR_FOREGROUND);
					m->inputs.insert(c);
					m->knob->SetIcon(knobicon[0], PIXMAP_CENTER, 0.5f);
					//m->label->SetColor(GetColor(WIDGETCOLOR_FOREGROUND), WIDGETCOLOR_FOREGROUND);
					EmitEvent(Event(EVENT_WIDGETCONNECT, Self(), 0, c->end, 0, c));
					return;
				}
				c->SetParent(NULL);
			}
			MouseMove(x, y);
		}
	}

	void FlowGraphNode::MouseUp(const MouseButton button, const int x, const int y)
	{
		if (button == MOUSE_LEFT)
		{
			mousepressed = false;
			auto parent = GetParent();
			auto graph = parent->As<FlowGraph>();
			if (graph)
			{
				graph->MouseUp(button, position.x + x, position.y + y);
			}
			MouseMove(x, y);
		}
	}

	void FlowGraphNode::MouseLeave(const int x, const int y)
	{
		auto graph = dynamic_pointer_cast<FlowGraph>(GetParent());
		if (graph)
		{
			auto m = graph->hoveredmethod.lock();
			if (m)
			{
				m->knob->SetIcon(FlowGraph::knobicon[0], PIXMAP_CENTER, 0.5f);
				m->label->SetColor(GetColor(WIDGETCOLOR_FOREGROUND), WIDGETCOLOR_FOREGROUND);
			}
			graph->hoveredmethod.reset();
		}
	}

	void FlowGraph::MouseMove(const int x, const int y)
	{
		if (mousepressed[MOUSE_MIDDLE])
		{
			int dx = x - mousedownposition[MOUSE_MIDDLE].x;;
			int dy = y - mousedownposition[MOUSE_MIDDLE].y;;
			offset.x += dx;
			offset.y += dy;
			for (auto child : kids)
			{
				if (not child->As<FlowGraphNode>()) continue;
				child->SetShape(child->position.x + dx, child->position.y + dy, child->size.x, child->size.y);
			}
			mousedownposition[MOUSE_MIDDLE].x = x;
			mousedownposition[MOUSE_MIDDLE].y = y;
			Redraw();
			return;
		}
		auto c = activeconnection.lock();
		if (c)
		{
			auto scale = GetInterface()->scale;
			auto m = GetMethodAtPoint(x, y, 1);
			auto prev = hoveredtarget.lock();
			if (prev and prev != m)
			{
				prev->knob->SetIcon(knobicon[0], PIXMAP_CENTER, 0.5f);
				prev->label->SetColor(1, 1, 1, 1, WIDGETCOLOR_FOREGROUND);
			}
			hoveredtarget.reset();
			if (m)
			{
				auto inputs = m->GetInputs();
				for (auto c2 : inputs)
				{
					if (c2->source.lock() == c->source.lock())
					{
						m = nullptr;
						break;
					}
				}
			}
			if (m)
			{
				m->knob->SetIcon(knobicon[1], PIXMAP_CENTER, 0.5f);
				m->label->SetColor(0, 1, 1, 1, WIDGETCOLOR_FOREGROUND);
				c->end.x = float(m->knob->GetParent()->GetParent()->position.x);// / scale;
				c->end.y = float(m->knob->GetPosition(true).y + m->knob->size.y / 2);// / scale;
				hoveredtarget = m;
			}
			else
			{
				c->end.x = float(x);// / scale;
				c->end.y = float(y);// / scale;
			}
			Redraw();
		}
	}

	void FlowGraphNode::MouseMove(const int x, const int y)
	{
		auto graph = GetParent()->As<FlowGraph>();
		if (graph)
		{
			auto coord = GetPosition();
			graph->MouseMove(coord.x + x, coord.y + y);
		}
		if (mousepressed)
		{
			auto paint = false;
			auto ui = GetInterface();
			float scale = ui->scale;
			int xx = Round(float(position.x + x - mouseclickposition.x) / 16.0f) * 16;
			int yy = Round(float(position.y + y - mouseclickposition.y) / 16.0f) * 16;
			if (xx == position.x and yy == position.y) return;
			SetShape(xx, yy, size.x, size.y);
			for (auto subnode : subnodes)
			{
				for (auto m : subnode->inputs)
				{
					for (auto wc : m->inputs)
					{
						auto c = wc.lock();
						if (not c) continue;
						c->end.x = float(m->knob->GetPosition(true).x + m->knob->size.x / 2);// / scale;
						c->end.y = float(m->knob->GetPosition(true).y + m->knob->size.y / 2);// / scale;
						paint = true;
					}
				}
				for (auto m : subnode->outputs)
				{
					for (auto c : m->outputs)
					{
						c->start.x = float(position.x + size.x);// / scale;
						c->start.y = float(m->knob->GetPosition(true).y + m->knob->size.y / 2);// / scale;
						paint = true;
					}
				}
			}
			if (paint) GetParent()->Redraw();
		}
		else if(x > -1 and y > -1 and x < size.x and y < size.y)
		{
			auto graph = GetParent()->As<FlowGraph>();
			if (graph)
			{
				auto m = graph->hoveredmethod.lock();
				if (m)
				{
					m->knob->SetIcon(FlowGraph::knobicon[0], PIXMAP_CENTER, 0.5f);
					m->label->SetColor(GetColor(WIDGETCOLOR_FOREGROUND), WIDGETCOLOR_FOREGROUND);
				}
				graph->hoveredmethod.reset();
			}
			int type = 1 + 2;
			auto a = graph->activeconnection.lock();
			if (a) type = 1;
			auto m = graph->GetMethodAtPoint(position.x + x, position.y + y, type);
			if (m)
			{
				if (a == nullptr and m->type == 1 and m->GetInputs().empty()) return;
				if (a)
				{
					auto inputs = m->GetInputs();
					for (auto c2 : inputs)
					{
						if (c2->source.lock() == a->source.lock()) return;
					}
				}
				m->knob->SetIcon(FlowGraph::knobicon[1], PIXMAP_CENTER, 0.5f);
				if (graph) graph->hoveredmethod = m;
				m->label->SetColor(0, 1, 1, 1, WIDGETCOLOR_FOREGROUND);
				return;
			}
		}
	}

	void FlowGraph::MouseDown(const MouseButton button, const int x, const int y)
	{
		mousepressed[button] = true;
		if (button == MOUSE_LEFT)
		{
			bool ctrl = false;
			auto ui = GetInterface();
			auto window = ui->GetWindow();
			if (window and window->KeyDown(KEY_CONTROL)) ctrl = true;
			if (not ctrl)
			{
				for (auto child : kids)
				{
					if (not child->As<FlowGraphNode>()) continue;
					child->SetState(false);
					EmitEvent(EVENT_WIDGETDESELECT, Self(), 0, 0, 0, 0, 0, child);
				}
			}
		}
		else if (button == MOUSE_MIDDLE)
		{
			mousedownposition[button].x = x;
			mousedownposition[button].y = y;
		}
	}

	shared_ptr<FlowGraphConnection> FlowGraphMethod::Connect(shared_ptr<FlowGraphMethod> target)
	{
		for (auto out : outputs)
		{
			if (out->dest.lock() == target) return out;
		}
		auto graph = label->GetParent()->GetParent()->GetParent();
		auto g = graph->As<FlowGraph>();
		auto c = CreateFlowGraphConnection(0,0,0,0,graph);
		c->SetColor(1, 0.85, 0, 1, WIDGETCOLOR_FOREGROUND);
		c->source = As<FlowGraphMethod>();
		c->dest = target;
		outputs.insert(c);
		target->inputs.insert(c);
		return c;
	}

	void FlowGraphNode::MouseDown(const MouseButton button, const int x, const int y)
	{
		if (x > -1 and y > -1 and x < size.x and y < size.y)
		{
			if (button == MOUSE_RIGHT)
			{
				auto graph = GetParent()->As<FlowGraph>();
				if (graph)
				{
					auto window = GetInterface()->GetWindow();
					if (window)
					{
						if (graph->popupmenu == NULL)
						{
							graph->popupmenu = CreateMenu("", graph->GetInterface());
							graph->menu_remove = CreateMenu("Remove", graph->popupmenu);							
							ListenEvent(EVENT_WIDGETACTION, graph->menu_remove, graph->EventHook, graph);
						}
						auto pos = GetPosition(true);
						pos.x += x;
						pos.y += y;
						pos = TransformCoord(pos, window, nullptr);
						PopupMenu(window, graph->popupmenu, pos.x, pos.y, Self());
					}
				}
			}
			else if (button == MOUSE_LEFT)
			{
				auto graph = GetParent()->As<FlowGraph>();
				if (graph)
				{					
					auto m = graph->hoveredmethod.lock();
					if (m)
					{						
						if (m->type == 1)
						{
							auto inputs = m->GetInputs();
							if (not inputs.empty())
							{
								graph->Lock();
								size_t i = pickindex % inputs.size();
								auto c = inputs[i];
								auto tar = c->dest.lock();
								if (tar)
								{
									EmitEvent(Event(EVENT_WIDGETDISCONNECT, graph, 0, 0, 0, c));
								}
								++pickindex;
								graph->activeconnection = c;
								graph->hoveredtarget = c->dest.lock();
								m->label->SetColor(0, 1, 1, 1, WIDGETCOLOR_FOREGROUND);
								m->knob->SetIcon(graph->knobicon[1], PIXMAP_CENTER, 0.5f);
								auto it = m->inputs.find(c);
								if (it != m->inputs.end()) m->inputs.erase(it);
								auto t = c->dest.lock();
								c->dest.reset();
								c->SetColor(0, 1, 1, 1, WIDGETCOLOR_FOREGROUND);
								c->MoveToTop();
								graph->Redraw();								
								graph->Unlock();
								return;
							}
						}
						else if (m->type == 2)
						{
							graph->Lock();
							iVec2 start = m->knob->GetPosition(true);
							iVec2 coord = GetPosition(true) + iVec2(x, y);
							start.x += m->knob->size.x / 2 + Round(1.0f * GetInterface()->scale);
							start.y += m->knob->size.y / 2;
							auto scale = GetInterface()->scale;
							auto c = CreateFlowGraphConnection(float(start.x), float(start.y), coord.x, coord.y, graph);;
							m->outputs.insert(c);
							c->source = m;
							c->end = c->start;
							graph->activeconnection = c;
							c->SetColor(0, 1, 1, 1, WIDGETCOLOR_FOREGROUND);
							graph->Unlock();
							return;
						}
					}
					graph->Lock();
					auto ui = GetInterface();
					auto window = ui->GetWindow();
					bool ctrl = false;
					if (window and window->KeyDown(KEY_CONTROL)) ctrl = true;
					if (not ctrl)
					{
						for (auto child : graph->kids)
						{
							if (child != Self() and child->GetState() == WIDGETSTATE_SELECTED)
							{
								child->SetState(false);
								EmitEvent(EVENT_WIDGETDESELECT, graph, 0, 0, 0, 0, 0, child);
							}
						}
					}
					if (ctrl)
					{
						bool state = true;
						if (GetState() == WIDGETSTATE_SELECTED) state = false;
						SetState(state);
						if (state)
						{
							EmitEvent(EVENT_WIDGETSELECT, graph, 0, 0, 0, 0, 0, Self());
						}
						else
						{
							EmitEvent(EVENT_WIDGETDESELECT, graph, 0, 0, 0, 0, 0, Self());
						}
					}
					else
					{
						if (GetState() != WIDGETSTATE_SELECTED)
						{
							SetState(true);
							EmitEvent(EVENT_WIDGETSELECT, graph, 0, 0, 0, 0, 0, Self());
						}
					}
					graph->Unlock();
				}
				mousepressed = true;
				mouseclickposition.x = x;
				mouseclickposition.y = y;
			}
			else if (button == MOUSE_RIGHT)
			{

			}
		}
	}

	bool FlowGraph::EventHook(const Event& e, shared_ptr<Object> extra)
	{
		auto graph = extra->As<FlowGraph>();
		if (e.id == EVENT_WIDGETACTION)
		{
			if (e.source == graph->menu_remove)
			{
				auto node = e.extra->As<FlowGraphNode>();
				node->Free();
			}
		}
		return true;
	}

	std::vector<shared_ptr<FlowGraphConnection> > FlowGraphMethod::GetInputs()
	{
		std::vector<shared_ptr<FlowGraphConnection> > r;
		for (auto wc : inputs)
		{
			auto c = wc.lock();
			if (c) r.push_back(c);
		}
		return r;
	}

	void FlowGraphNode::Draw(const int x, const int y, const int width, const int height)
	{
		auto ui = GetInterface();
		float scale = 1;
		if (ui) scale = ui->scale;
		blocks.clear();
		int r = Round(4.0f * scale);
		int titleheight = Round(18.0f * scale);
		int itemheight = Round(18.0f * scale);

		//Background
		AddBlock(iVec2(0), size, GetParent()->GetColor(WIDGETCOLOR_BORDER), false, r);
		//AddBlock(iVec2(0), size, GetColor(), false, r);
		AddBlock(0, size, Vec4(1,0,0,1), true, r);
		AddBlock(iVec2(0, titleheight), iVec2(size.x, size.y - titleheight), GetColor(), false, r);
		AddBlock(iVec2(0, titleheight), iVec2(size.x, size.y - titleheight - r), GetColor(), false);

		//Name
		AddBlock(text, 0, iVec2(size.x, itemheight), GetColor(WIDGETCOLOR_FOREGROUND), TEXT_CENTER | TEXT_MIDDLE);
		SetFontBold(true);

		if (background == NULL)
		{
			background = CreatePixmap(256,4);
			int a = 255;
			int c, r, g, b;
			for (int x = 0; x < background->size.x; ++x)
			{
				r = Round(float(x + 64) * 0.125f);
				g = r * 1.5f;
				b = r * 1.5f;
				c = Rgba(r,g,b,255);
				background->WritePixel(x, 0, c);
				background->WritePixel(x, 1, c);
				background->WritePixel(x, 2, c);
				background->WritePixel(x, 3, c);
			}
		}

		//blocks[0].SetPixmap(background);
		//blocks[1].SetPixmap(background);
		//blocks[2].SetPixmap(background);

		//Subnodes
		int py = titleheight;
		for (auto& subnode : subnodes)
		{
			//AddBlock(iVec2(0, py), iVec2(size.x, titleheight + subnode->inputs.size() * itemheight + Round(8.0f * scale)), GetColor(), false, 0, -0.75);
			//AddBlock(iVec2(0, py), iVec2(size.x, titleheight + subnode->inputs.size() * itemheight + Round(8.0f * scale)), GetColor());
			//py = subnode->panel->position.y + subnode->panel->size.y;
	//		AddBlock(subnode->name, iVec2(0, py), iVec2(size.x, titleheight), GetColor(WIDGETCOLOR_FOREGROUND), TEXT_MIDDLE | TEXT_CENTER);
//
		//	for (auto input : subnode->inputs)
		//	{
		//		py += itemheight;
		//		AddBlock(input->name, iVec2(0, py), iVec2(size.x - Round(16.0f * scale), titleheight), GetColor(WIDGETCOLOR_FOREGROUND), TEXT_MIDDLE | TEXT_RIGHT);
		//	}
		}

		//Border
		Vec4 bcolor = GetColor(WIDGETCOLOR_BORDER);
		if (GetState() == WIDGETSTATE_SELECTED) bcolor = GetColor(WIDGETCOLOR_SELECTION);
		AddBlock(0, size, bcolor, true, 4.0f * scale, 0.65f);
	}

	void FlowGraphSubNode::Free()
	{
		for (auto m : inputs)
		{
			auto inputs = m->inputs;
			for (auto wc : inputs)
			{
				auto c = wc.lock();
				if (c) c->Free();
			}
		}
		for (auto m : outputs)
		{
			auto outputs = m->outputs;
			for (auto c : outputs)
			{
				c->Free();
			}
		}
		auto parent = panel->GetParent();
		auto node = dynamic_pointer_cast<FlowGraphNode>(parent);
		if (node)
		{
			for (auto it = node->subnodes.begin(); it != node->subnodes.end(); ++it)
			{
				if (*it == Self())
				{
					node->subnodes.erase(it);
					break;
				}
			}
		}
		panel->SetParent(NULL);
		SetParent(nullptr, 0);
	}

	void FlowGraphNode::Free()
	{
		extra = nullptr;
		for (auto subnode : subnodes)
		{
			for (auto m : subnode->inputs)
			{
				for (auto wc : m->inputs)
				{
					auto c = wc.lock();
					if (c) c->SetParent(NULL);
				}
				for (auto c : m->outputs)
				{
					if (c) c->SetParent(NULL);
				}
			}
			for (auto m : subnode->outputs)
			{
				for (auto wc : m->inputs)
				{
					auto c = wc.lock();
					if (c) c->SetParent(NULL);
				}
				for (auto c : m->outputs)
				{
					if (c) c->SetParent(NULL);
				}
			}
			subnode->outputs.clear();
			subnode->inputs.clear();
		}
		SetParent(NULL);
	}

	void FlowGraphConnection::Draw(const int x, const int y, const int w, const int h)
	{
		float scale = 1;
		auto ui = GetInterface();
		if (ui) scale = ui->scale;
		points.clear();
		//points.push_back(start);

		blocks.clear();

		auto src = this->source.lock();
		auto dest = this->dest.lock();

		if (src == nullptr) return;

		auto node = src->knob->GetParent()->GetParent();
		start.x = node->position.x + node->size.x;
		start.y = src->knob->GetPosition(true).y + src->knob->size.y / 2;
		//start.x *= scale;
		//start.y *= scale;
		
		if (dest)
		{
			node = dest->knob->GetParent()->GetParent();
			end.x = node->position.x;
			end.y = dest->knob->GetPosition(true).y + dest->knob->size.y / 2;
			//end.x *= scale;
			//end.y *= scale;
		}

		int len = Min(Vec2(start.x, start.y).DistanceToPoint(Vec2(end.x, end.y)) * 0.5f, 500.0f);
		bezier::Bezier<3> cubicBezier({ {double(start.x), double(start.y) }, {double(start.x + len), double(start.y)}, {double(end.x - len), double(end.y)}, {double(end.x), double(end.y)} });

		// Get coordinates on the curve from a value between 0 and 1 (values outside this range are also valid because of the way bezier curves are defined).
		bezier::Point p;

		float l = cubicBezier.length();
		int segs = Round(l / 20.0f);
		segs = Max(segs, 2);

		for (int n = 0; n <= segs; ++n)
		{
			p = cubicBezier.valueAt(float(n) / float(segs));// *scale;
			points.push_back(iVec2(p.x, p.y));
		}

		blocks.clear();
		AddBlock(points, GetColor(WIDGETCOLOR_FOREGROUND));
		blocks[blocks.size() - 1].linethickness = 3;
	}

	void FlowGraphConnection::Free()
	{
		auto src = this->source.lock();
		if (src)
		{
			auto it = src->outputs.find(As<FlowGraphConnection>());
			if (it != src->outputs.end()) src->outputs.erase(it);
		}
		auto dst = this->dest.lock();
		if (dst)
		{
			auto it = src->inputs.find(As<FlowGraphConnection>());
			if (it != src->inputs.end()) src->inputs.erase(it);
		}
		SetParent(NULL);
	}

	void FlowGraphConnection::MoveToTop()
	{
		auto parent = GetParent();
		SetParent(NULL);
		int index = 0;
		for (auto child : parent->kids)
		{
			if (not child->As<FlowGraphConnection>()) break;
			index++;
		}
		SetParent(parent, index);
	}

	void FlowGraphNode::MoveToTop()
	{
		auto parent = GetParent();
		SetParent(NULL);
		SetParent(parent);
		/*int index = 0;
		for (auto child : parent->kids)
		{
			if (not child->As<FlowGraphConnection>()) break;
			index++;
		}
		SetParent(parent, index);*/
	}

	shared_ptr<FlowGraphConnection> CreateFlowGraphConnection(const int x0, const int y0, const int x1, const int y1, shared_ptr<Widget> parent)
	{
		auto fg = std::make_shared<FlowGraphConnection>();
		fg->Initialize("", 0, 0, parent->size.x, parent->size.y, parent, 0);
		fg->SetLayout(1, 1, 1, 1);
		fg->SetInteractive(false);
		fg->start = iVec2(x0, y0);
		fg->end = iVec2(x1, y1);
		fg->MoveToTop();
		return fg;
	}
}