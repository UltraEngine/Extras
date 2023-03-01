#include "UltraEngine.h"
#include "ButtonStrip.h"

namespace UltraEngine
{
	ButtonStrip::ButtonStrip() : hovereditem(-1), pressed(false), presseditem(-1)
	{}

	void ButtonStrip::Init(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent)
	{
		Widget::Initialize(L"", x, y, width, height, parent, 0);
		SetColor(GetColor(WIDGETCOLOR_SELECTION), WIDGETCOLOR_FOREGROUND);
		cornerradius = height / 2;
	}

	shared_ptr<Widget> CreateButtonStrip(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent)
	{
		auto widget = std::make_shared<ButtonStrip>();
		widget->Init(x, y, width, height, parent);
		return widget;
	}

	void ButtonStrip::MouseMove(const int x, const int y)
	{
		if (x < 0 or y < 0 or x >= size.x or y >= size.y) return;
		auto temp = hovereditem;
		int bw = Round(float(size.x) / float(items.size()));
		hovereditem = Floor(float(x) / float(bw));
		if (hovereditem < 0 or hovereditem > items.size()) hovereditem = -1;
		if (temp != hovereditem) Redraw();
	}

	void ButtonStrip::MouseEnter(const int x, const int y)
	{
	}

	void ButtonStrip::MouseLeave(const int x, const int y)
	{
		if (hovereditem == -1) return;
		hovereditem = -1;
		Redraw();
	}
	
	void ButtonStrip::MouseDown(const MouseButton button, const int x, const int y)
	{
		if (button != MOUSE_LEFT or hovereditem == -1) return;
		pressed = true;
		presseditem = hovereditem;
		Redraw();
	}
	
	void ButtonStrip::MouseUp(const MouseButton button, const int x, const int y)
	{
		if (hovereditem != -1)
		{
			if (GetSelectedItem() == hovereditem)
			{
				//SelectItem(-1);
				EmitEvent(EVENT_WIDGETACTION, Self(), presseditem);
			}
			else
			{
				//SelectItem(hovereditem);
				EmitEvent(EVENT_WIDGETACTION, Self(), presseditem);
			}
		}
		presseditem = -1;
		pressed = false;
		Redraw();
	}

	void ButtonStrip::Draw(const int x, const int y, const int width, const int height)
	{
		blocks.resize(8 + items.size() * 3);

		//Background
		blocks[0].size = this->size;
		blocks[0].radius = gui->scale * cornerradius; //Round(float(this->size.y) * 0.5f);
		blocks[0].color = color[WIDGETCOLOR_BACKGROUND];
		blocks[0].gradient = 0.75;
		blocks[0].hidden = false;

		//Border
		blocks[1].size = this->size;
		blocks[1].radius = gui->scale * cornerradius;//Round(float(this->size.y) * 0.5f);
		blocks[1].color = color[WIDGETCOLOR_BORDER];
		blocks[1].wireframe = true;
		blocks[1].hidden = false;

		int bw = Round(float(size.x) / float(items.size()));

		//Selected outer button background
		blocks[2].size.y = this->size.y;
		blocks[2].size.x = bw + blocks[0].radius*2;
		blocks[2].color = color[WIDGETCOLOR_SUNKEN];
		blocks[2].hidden = true;
		blocks[2].radius = blocks[0].radius;
		blocks[2].hidden = true;

		//We need one for left and one for right
		blocks[3].size.y = this->size.y;
		blocks[3].size.x = bw + blocks[0].radius * 2;
		blocks[3].position.x = size.x - blocks[3].size.x;
		blocks[3].color = color[WIDGETCOLOR_SUNKEN];
		blocks[3].hidden = true;
		blocks[3].radius = blocks[0].radius;
		blocks[3].hidden = true;

		//Outer button rounded border
		blocks[4].size.x = bw + blocks[0].radius + 1;
		blocks[4].position.x = size.x - blocks[4].size.x;
		blocks[4].size.y = this->size.y;
		blocks[4].radius = gui->scale * cornerradius; ;// Round(float(this->size.y) * 0.5f);
		blocks[4].color = color[WIDGETCOLOR_SELECTION];
		blocks[4].wireframe = true;
		blocks[4].hidden = true;

		int i = hovereditem;
		if (i == 0)
		{
			//blocks[4].position.x = 0;
		}
		else if (i == items.size()-1)
		{
			//blocks[4].position.x = size.x -  blocks[2].size.x;
		}
		else
		{
			//blocks[4].hidden = true;
		}

		//Center panel to hide inner border radius
		blocks[5].position.x = bw;
		blocks[5].size.y = this->size.y;
		blocks[5].size.x = size.x - bw * 2;
		blocks[5].color = color[WIDGETCOLOR_BACKGROUND];
		blocks[5].gradient = 0.75;
		blocks[5].hidden = false;
		if (items.size() == 2 and hovereditem != -1)
		{
			blocks[5].position.x++;
			blocks[5].size.x = bw - Round(float(size.y) * 0.5f);
			if (hovereditem == 1) blocks[5].position.x = Round(float(size.y) * 0.5f);
		}

		//Selected button background
		blocks[6].size.y = this->size.y;
		blocks[6].size.x = bw;
		blocks[6].color = color[WIDGETCOLOR_SUNKEN];
		blocks[6].hidden = true;
		blocks[6].radius = 0;

		i = 7;

		for (int n = 0; n < items.size(); ++n)
		{
			if (n > 0 and n < items.size() - 1)
			{
				blocks[i + n * 3 + 0].wireframe = true;
				blocks[i + n * 3 + 0].color = color[WIDGETCOLOR_BORDER];
				if (hovereditem == n) blocks[i + n * 3 + 0].color = color[WIDGETCOLOR_SELECTION];
				blocks[i + n * 3 + 0].hidden = false;
				blocks[i + n * 3 + 0].size.x = bw;
				blocks[i + n * 3 + 0].size.y = size.y;
				blocks[i + n * 3 + 0].position.x = n * bw;
			}
			else
			{
				blocks[i + n * 3 + 0].hidden = true;
			}
			if (hovereditem == n)
			{
				blocks[4].position.x = 0;
				if (n==0) blocks[4].hidden = false;
				if (n == items.size() - 1)
				{
					blocks[4].hidden = false;
					blocks[4].position.x = size.x - blocks[4].size.x;
				}
			}
			blocks[i + n * 3 + 1].hidden = true;
			if (GetSelectedItem() == n or presseditem == n or items[n].state == WIDGETSTATE_SELECTED)
			{
				if (n==0 or n==items.size()-1)
				{
					//blocks[4].hidden = false;
					//blocks[4].position.x = 0;
					if (n == 0) blocks[2].hidden = false;
					if (n != 0)
					{
						blocks[3].hidden = false;
						//blocks[4].position.x = size.x - blocks[4].size.x;
					}
					blocks[6].radius = blocks[0].radius;
				}
				else
				{
					blocks[i + n * 3 + 1].position.x = n * bw + 1;
					blocks[i + n * 3 + 1].position.y = 1;
					blocks[i + n * 3 + 1].size.x = bw - 2;
					blocks[i + n * 3 + 1].size.y = size.y - 2;
					blocks[i + n * 3 + 1].color = color[WIDGETCOLOR_SUNKEN];
					blocks[i + n * 3 + 1].hidden = false;
					blocks[6].position.x = n * bw;
					blocks[6].hidden = false;
				}
			}
			if (items[n].icon)
			{
				blocks[i + n * 3 + 2].color = color[WIDGETCOLOR_FOREGROUND];
				if (GetSelectedItem() == n or items[n].state == WIDGETSTATE_SELECTED)// or presseditem == n)
				{
					blocks[i + n * 3 + 2].color = Vec4(1,1,1,1);// color[WIDGETCOLOR_SELECTION];
				}
				blocks[i + n * 3 + 2].iconscale = fontscale;
				blocks[i + n * 3 + 2].icon = items[n].icon;
				blocks[i + n * 3 + 2].size.x = Round(float(items[n].icon->size.x) * fontscale);
				blocks[i + n * 3 + 2].size.y = Round(float(items[n].icon->size.y) * fontscale);
				blocks[i + n * 3 + 2].hidden = false;
				blocks[i + n * 3 + 2].position.x = n * bw + Round(float(bw - blocks[i + n * 3 + 2].size.x) * 0.5f);
				blocks[i + n * 3 + 2].position.y = Round(float(size.y - blocks[i + n * 3 + 2].size.y) * 0.5f);
			//	if (n == 0) blocks[i + n * 3 + 2].position.x += Round(float(size.y) * 0.1f);
			//	if (n == items.size()-1) blocks[i + n * 3 + 2].position.x -= Round(float(size.y) * 0.1f);
			}
			else
			{
				blocks[i + n * 3 + 1].hidden = true;
			}
		}

		//Inner edge of outer highlighted buttons
		blocks[blocks.size() - 1].position.x = bw;
		blocks[blocks.size() - 1].size.x = 1;;
		blocks[blocks.size() - 1].size.y = this->size.y;
		blocks[blocks.size() - 1].color = color[WIDGETCOLOR_SELECTION];
		blocks[blocks.size() - 1].hidden = true;
		if (hovereditem == items.size() - 1)
		{
			blocks[blocks.size() - 1].position.x = bw * (items.size()-1);
			blocks[blocks.size() - 1].hidden = false;
		}
		if (hovereditem == 0)
		{
			blocks[blocks.size() - 1].hidden = false;
		}

		if (items.size() == 2 and hovereditem != -1)
		{
			AddBlock(blocks[5].position, iVec2(blocks[5].size.x, 1), color[WIDGETCOLOR_BORDER]);
			AddBlock(iVec2(blocks[5].position.x, size.y-1), iVec2(blocks[5].size.x, 1), color[WIDGETCOLOR_BORDER]);
		}
	}
}
