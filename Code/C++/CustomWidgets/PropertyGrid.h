#pragma once;
#include "UltraEngine.h"
#include "ScrollPanel.h"
#include "ColorEdit.h"
#include "Hyperlink.h"
#include "PathEdit.h"

namespace UltraEngine
{
	enum PropertyType
	{
		PROPERTY_INTEGER,
		PROPERTY_NUMBER,
		PROPERTY_VECTOR2,
		PROPERTY_VECTOR3,
		PROPERTY_VECTOR4,
		PROPERTY_ANGLE,
		PROPERTY_COLOR,
		PROPERTY_BOOLEAN,
		PROPERTY_STRING,
		PROPERTY_OPTION,
		PROPERTY_FILE,
		PROPERTY_DIR,
		PROPERTY_LABEL,
		PROPERTY_URL
	};

	class PropertyEdit : public Object
	{
	public:
		PropertyType type;
		shared_ptr<Widget> label, widget;
	};

	class PropertyGrid;

	class PropertyGroup : public Object
	{
		bool openstate;
		std::weak_ptr<PropertyGrid> grid;
		shared_ptr<Widget> header, panel, label, toggle, icon;
	public:
		PropertyGroup();
		virtual void Expand(), Collapse(), Toggle();
		shared_ptr<Widget> AddProperty(const WString& name, const PropertyType type);
		friend PropertyGrid;
	};

	class PropertyGrid : public Widget
	{
		static bool EventCallback(const Event&, shared_ptr<Object>);
		static shared_ptr<Icon> icon_open, icon_closed;
		bool ProcessEvent(const Event& e);
		virtual void Arrange();
		shared_ptr<Widget> gutter;
		shared_ptr<ScrollPanel> panel;
		std::vector<shared_ptr<PropertyGroup> > groups;
	public:
		virtual shared_ptr<PropertyGroup> AddGroup(const WString& name, shared_ptr<Icon> icon=NULL);
		virtual void UpdateLayout();

		friend PropertyGroup;
		friend shared_ptr<PropertyGrid> CreatePropertyGrid(const int, const int, const int, const int, shared_ptr<Widget>);
	};

	extern shared_ptr<PropertyGrid> CreatePropertyGrid(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent);
};