#include "UltraEngine.h"
#include "PropertyGrid.h"

namespace UltraEngine
{
	shared_ptr<Icon> PropertyGrid::icon_open, PropertyGrid::icon_closed;

//	s = "<?xml version=\"1.0\" encoding=\"utf-8\"?> <!-- Generator: Adobe Illustrator 16.0.3, SVG Export Plug-In . SVG Version: 6.00 Build 0) --> <!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"> <svg version=\"1.1\" id=\"Layer_2\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\" y=\"0px\" width=\"13px\" height=\"13px\" viewBox=\"0 0 13 13\" enable-root=\"new 0 0 13 13\" xml:space=\"preserve\"> <polygon fill=\"#FFFFFF\" points=\"2.202,10.781 10.797,10.797 10.797,2.203 \"/> </svg>";
//	buffer->Resize(s.length() + 1);
//	buffer->PokeString(0, s);
//	icon_treeviewnode_open = LoadIcon(stream);

//	s = "<?xml version=\"1.0\" encoding=\"utf-8\"?> <!-- Generator: Adobe Illustrator 16.0.3, SVG Export Plug-In . SVG Version: 6.00 Build 0) --> <!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"> <svg version=\"1.1\" id=\"Layer_2\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\" y=\"0px\" width=\"10px\" height=\"13px\" viewBox=\"0 0 10 13\" enable-root=\"new 0 0 10 13\" xml:space=\"preserve\"> <polygon fill=\"#FFFFFF\" points=\"1.922,12.5 8.078,6.501 1.922,0.5 \"/> </svg> ";
//	buffer->Resize(s.length() + 1);
//	buffer->PokeString(0, s);
//	icon_treeviewnode_closed = LoadIcon(stream);

	shared_ptr<Widget> PropertyGroup::AddProperty(const WString& name, const PropertyType type)
	{
		int x = 8;
		int h = 28;
		int y = panel->kids.size() * h + 1;
		//int x, y, h;
		int lw = 100;
		int endpadding = Round(6.0f * panel->gui->scale);

		auto box = CreatePanel(0,y+1,panel->size.x-2,h - 2,panel,PANEL_BORDER);
		box->SetColor(box->GetColor(WIDGETCOLOR_BACKGROUND), WIDGETCOLOR_BORDER);
		box->SetColor(0, 0, 0, 0); //no background
		box->SetLayout(1, 1, 1, 0);

		y = 4;

		label = CreateLabel(name, x, y, lw, h + 2, box);
		label->SetLayout(1,0,1,0);

		panel->SetShape(panel->position.x, panel->position.y, panel->size.x, panel->size.y + 28);

		auto cs = box->ClientSize();
		cs.x -= endpadding;

		shared_ptr<Widget> widget;
		switch (type)
		{
		case PROPERTY_OPTION:
			widget = CreateComboBox(x + lw, 0, cs.x - x - lw, cs.y - 1, box);
			break;
		case PROPERTY_LABEL:
			widget = CreateLabel("", x + lw, 0, cs.x - x - lw, cs.y - 1, box);
			break;
		case PROPERTY_URL:
			widget = CreateHyperlink("", x + lw, 0, cs.x - x - lw, cs.y - 1, box);
			break;
		case PROPERTY_VECTOR2:
			widget = CreateVectorEdit(x + lw, 0, cs.x - x - lw, cs.y - 1, box, VECTOREDIT_2D);
			break;
		case PROPERTY_VECTOR3:
			widget = CreateVectorEdit(x + lw, 0, cs.x - x - lw, cs.y - 1, box, VECTOREDIT_3D);
			break;
		case PROPERTY_VECTOR4:
			widget = CreateVectorEdit(x + lw, 0, cs.x - x - lw, cs.y - 1, box, VECTOREDIT_4D);
			break;
		case PROPERTY_COLOR:
			widget = CreateColorEdit(x + lw, 0, cs.x - x - lw, cs.y - 1, box);
			break;
		case PROPERTY_NUMBER:
			widget = CreateSpinner(x + lw, 0, cs.x - x - lw, cs.y-1, box, SPINNER_FLOAT);
			break;
		case PROPERTY_BOOLEAN:
			widget = CreateButton("False",x + lw,y,panel->size.x-x,h, box, BUTTON_CHECKBOX);
			break;
		case PROPERTY_FILE:
			widget = CreatePathEdit(x + lw, 0, cs.x - x - lw, cs.y - 1, box, PATHEDIT_FILE);
			break;
		case PROPERTY_DIR:
			widget = CreatePathEdit(x + lw, 0, cs.x - x - lw, cs.y - 1, box, PATHEDIT_FOLDER);
			break;
		default:
			widget = CreateTextField(x + lw, 0, cs.x - x - lw, cs.y - 1, box);
			widget->SetColor(0, 0, 0, 0, WIDGETCOLOR_BACKGROUND);
			widget->SetColor(0, 0, 0, 0, WIDGETCOLOR_BORDER);
			widget->SetColor(0, 0, 0, 0, WIDGETCOLOR_SELECTION);
			break;
		}
		if (widget)
		{
			//widget->SetColor(widget->color[WIDGETCOLOR_POPUP], WIDGETCOLOR_SUNKEN);
			widget->SetLayout(1, 1, 1, 0);
			auto grid = this->grid.lock();
			ListenEvent(EVENT_WIDGETACTION, widget, PropertyGrid::EventCallback, grid);
			ListenEvent(EVENT_WIDGETSELECT, widget, PropertyGrid::EventCallback, grid);
		}
		return widget;
	}

	PropertyGroup::PropertyGroup()
	{
		openstate = true;
	}

	void PropertyGrid::UpdateLayout()
	{
		auto sz = size;
		Widget::UpdateLayout();
		if (sz.y != size.y) Arrange();
	}

	void PropertyGrid::Arrange()
	{
		int y = -1;
		for (const auto& group : groups)
		{
			group->header->SetShape(-1, y, group->header->size.x+2, group->header->size.y);
			if (group->openstate)
			{
				group->panel->SetShape(0, group->header->position.y + group->header->size.y, group->panel->size.x, group->panel->size.y);
				y = group->panel->position.y + group->panel->size.y;
			}
			else
			{
				y = group->header->position.y + group->header->size.y - 1;
			}
		}
		panel->SetArea(panel->size.x, y);
	}

	shared_ptr<PropertyGroup> PropertyGrid::AddGroup(const WString& name, shared_ptr<Icon> icon)
	{
		int y = 0;
		for (auto g : groups)
		{
			y = Max(y, g->panel->position.y + g->panel->size.y);
		}

		//if (groups[name] != NULL) return groups[name];
		auto group = std::make_shared<PropertyGroup>();
		group->header = CreateButton("", -1, y, panel->subpanel->size.x+2, 28, panel->subpanel);// , PANEL_BORDER);
		group->header->SetRadius(0);
		group->header->SetColor(group->header->GetColor(WIDGETCOLOR_RAISED));
		group->header->SetColor(group->header->GetColor(WIDGETCOLOR_BORDER), WIDGETCOLOR_SELECTION);
		group->header->SetLayout(1, 1, 1, 0);
		group->label = CreateLabel(name, 4 + 20 + 20 * int(icon!= NULL), 0, 200, group->header->size.y, group->header, LABEL_MIDDLE);
		group->label->SetFontBold(true);
		group->label->SetInteractive(false);
		group->grid = As<PropertyGrid>();

		ListenEvent(EVENT_WIDGETACTION, group->header, bind(&PropertyGroup::Toggle,group));

		if (icon_open == NULL)
		{
			auto buffer = CreateBuffer();
			auto stream = CreateBufferStream(buffer);

			String s = "<?xml version=\"1.0\" encoding=\"utf-8\"?> <!-- Generator: Adobe Illustrator 16.0.3, SVG Export Plug-In . SVG Version: 6.00 Build 0) --> <!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"> <svg version=\"1.1\" id=\"Layer_2\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\" y=\"0px\" width=\"13px\" height=\"13px\" viewBox=\"0 0 13 13\" enable-root=\"new 0 0 13 13\" xml:space=\"preserve\"> <polygon fill=\"#FFFFFF\" points=\"2.202,10.781 10.797,10.797 10.797,2.203 \"/> </svg>";
			buffer->Resize(s.length() + 1);
			buffer->PokeString(0, s);
			icon_open = LoadIcon(stream);

			s = "<?xml version=\"1.0\" encoding=\"utf-8\"?> <!-- Generator: Adobe Illustrator 16.0.3, SVG Export Plug-In . SVG Version: 6.00 Build 0) --> <!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"> <svg version=\"1.1\" id=\"Layer_2\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\" y=\"0px\" width=\"10px\" height=\"13px\" viewBox=\"0 0 10 13\" enable-root=\"new 0 0 10 13\" xml:space=\"preserve\"> <polygon fill=\"#FFFFFF\" points=\"1.922,12.5 8.078,6.501 1.922,0.5 \"/> </svg> ";
			buffer->Resize(s.length() + 1);
			buffer->PokeString(0, s);
			icon_closed = LoadIcon(stream);
		}

		group->toggle = CreatePanel(0, 0, 20, 28, group->header);
		group->toggle->SetColor(0, 0, 0, 0);
		group->toggle->SetIcon(icon_open,PIXMAP_CENTER,0.5f*panel->gui->scale);
		group->toggle->SetInteractive(false);

		if (icon)
		{
			group->icon = CreatePanel(20, 0, 20, 28, group->header);
			group->icon->SetColor(0, 0, 0, 0);
			group->icon->SetIcon(icon, PIXMAP_CENTER);
			group->icon->SetInteractive(false);
		}

		group->panel = CreatePanel(0, group->header->position.y + group->header->size.y, panel->subpanel->size.x-0, 2, panel->subpanel);
		group->panel->SetLayout(1, 1, 1, 0);
		group->panel->SetColor(0, 0, 0, 0);

		groups.push_back(group);
		return group;
	}

	void PropertyGroup::Expand()
	{
		if (openstate) return;
		openstate = true;
		toggle->SetIcon(PropertyGrid::icon_open, PIXMAP_CENTER, 0.5 * toggle->gui->scale);
		auto grid = this->grid.lock();
		if (grid)
		{

			grid->Arrange();

		}
		auto parent = panel->GetParent();
		auto sz = parent->ClientSize();
		panel->SetShape(panel->position.x, panel->position.y, sz.x, panel->size.y);
		panel->SetLayout(1, 1, 1, 0);
		panel->SetHidden(false);
	}
	
	void PropertyGroup::Collapse()
	{
		if (!openstate) return;
		openstate = false;
		toggle->SetIcon(PropertyGrid::icon_closed, PIXMAP_CENTER, 0.5 * toggle->gui->scale);
		panel->SetHidden(true);
		panel->SetLayout(1, 0, 1, 0);
		auto grid = this->grid.lock();
		if (grid) grid->Arrange();
	}

	void PropertyGroup::Toggle() { if (openstate) Collapse(); else Expand(); }

	bool PropertyGrid::ProcessEvent(const Event& e)
	{
		switch (e.id)
		{
		case EVENT_WIDGETSELECT:
		case EVENT_WIDGETACTION:
			if (e.source->As<Button>() != NULL)
			{
				auto widget = e.source->As<Widget>();
				if ((widget->style & BUTTON_CHECKBOX) != 0)
				{
					if (e.data == 1)
					{
						widget->SetText("True");
					}
					else
					{
						widget->SetText("False");
					}
				}
			}
			Event e2 = e;
			e2.id = EVENT_WIDGETACTION;
			e2.source = Self();
			e2.extra = e.source;
			EmitEvent(e2);
			break;
		}
		return true;
	}

	bool PropertyGrid::EventCallback(const Event& e, shared_ptr<Object> extra)
	{
		auto grid = extra->As<PropertyGrid>();
		if (grid) return grid->ProcessEvent(e);
		return false;
	}

	shared_ptr<PropertyGrid> CreatePropertyGrid(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent)
	{
		auto widget = std::make_shared<PropertyGrid>();
		widget->Initialize(L"", x, y, width, height, parent, 0);
		widget->panel = CreateScrollPanel(0, 0, width, height, widget);
		widget->panel->SetLayout(1, 1, 1, 1);
		widget->gutter = CreatePanel(0, 0, 28, widget->panel->subpanel->size.y, widget->panel->subpanel);
		widget->gutter->SetColor(0, 0, 0, 0);
		//widget->gutter->SetColor(widget->panel->color[WIDGETCOLOR_POPUP]);
		widget->gutter->SetLayout(1, 0, 1, 1);
		widget->gutter->SetHidden(true);
		//widget->panel->subpanel->SetColor(widget->color[WIDGETCOLOR_SUNKEN]);
		return widget;
	}
}