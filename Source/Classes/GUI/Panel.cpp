#include "UltraEngine.h"

namespace UltraEngine
{
	int grouppadding=20;

	Panel::Panel()
	{
		classname = "Panel";
		//color[WIDGETCOLOR_BACKGROUND] = Vec4(0.17,0.17,0.17,1);
		color[WIDGETCOLOR_BACKGROUND] = Vec4(0.2, 0.2, 0.2, 1);
		color[WIDGETCOLOR_BORDER] = Vec4(0, 0, 0, 1);
		color[WIDGETCOLOR_FOREGROUND] = Vec4(1, 1, 1, 1);
		blocks.resize(5);
	}
	
	shared_ptr<Panel> CreatePanel(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const PanelStyle style)
	{
		auto widget = make_shared<Panel>();
		//parent->kids.push_back(widget);
		//widget->parent = parent;
		widget->position.x = x; widget->position.y = y; widget->size.x = width; widget->size.y = height;
		widget->gui = parent->gui;
		widget->edgedistance[0] = x;
		widget->edgedistance[2] = y;
		widget->style = style;
		widget->Initialize("", x, y, width, height, parent, style);
		if ((style & PANEL_BORDER) != 0) widget->SetPadding(1, 1, 1, 1);
		if ((style & PANEL_GROUP) != 0)
		{
			widget->SetPadding(grouppadding, grouppadding, grouppadding, grouppadding);
			widget->color[WIDGETCOLOR_BORDER] = Vec4(0.9, 0.9, 0.9, 1);
		}
		widget->Invalidate();
		widget->Redraw();
		auto pos = widget->GetPosition(true);
		widget->gui->Redraw(pos.x, pos.y, widget->size.x, widget->size.y);
		return widget;
	}

	/*shared_ptr<Panel> CreatePanel(const int x, const int y, const int width, const int height, shared_ptr<Interface> ui, const PanelStyle style)
	{
		auto widget = make_shared<Panel>();
		//ui->rootwidgets.push_back(widget);
		//widget->it = ui->rootwidgets.end();
		widget->Initialize("", x, y, width, height, ui, style);
		//widget->it--;
		widget->position.x = x; widget->position.y = y; widget->size.x = width; widget->size.y = height;
		widget->gui = ui;
		widget->edgedistance[0] = x;
		widget->edgedistance[2] = y;
		widget->style = style;
		//widget->Initialize(x, y, width, height, ui, style);
		if ((style & PANEL_BORDER) != 0) widget->SetPadding(1, 1, 1, 1);
		if ((style & PANEL_GROUP) != 0)
		{
			widget->SetPadding(grouppadding, grouppadding, grouppadding, grouppadding);
			widget->color[WIDGETCOLOR_BORDER] = Vec4(0.9, 0.9, 0.9, 1);
		}
		widget->Invalidate();
		auto pos = widget->GetPosition(true);
		widget->gui->Redraw(pos.x,pos.y, widget->size.x, widget->size.y);
		return widget;
	}*/

	/*void Panel::MouseWheel(const int delta, const int x, const int y) { EmitEvent(EVENT_MOUSEWHEEL, Self(), delta, x, y); };
	void Panel::MouseEnter(const int x, const int y) { Widget::MouseEnter(x, y); EmitEvent(EVENT_MOUSEENTER, Self(), 0, x, y); }
	void Panel::MouseLeave(const int x, const int y) { Widget::MouseLeave(x, y); EmitEvent(EVENT_MOUSELEAVE, Self(), 0, x, y);  }
	void Panel::MouseMove(const int x, const int y) { Widget::MouseMove(x,y);  EmitEvent(EVENT_MOUSEMOVE, Self(), 0, x, y); }
	void Panel::MouseDown(const MouseButton button, const int x, const int y) { EmitEvent(EVENT_MOUSEDOWN, Self(), button, x, y); }
	void Panel::MouseUp(const MouseButton button, const int x, const int y) { EmitEvent(EVENT_MOUSEUP, Self(), button, x, y); }
	void Panel::DoubleClick(const MouseButton button, const int x, const int y) { EmitEvent(EVENT_DOUBLECLICK, Self(), button, x, y); }
	void Panel::TripleClick(const MouseButton button, const int x, const int y) { EmitEvent(EVENT_TRIPLECLICK, Self(), button, x, y); }*/

	void Panel::Draw(const int x, const int y, const int width, const int height)
	{
		float aspect, imageaspect;

		for (auto& block : blocks)
		{
			block.hidden = true;
		}

		//Background
		blocks[0].color = color[WIDGETCOLOR_BACKGROUND];
		blocks[0].wireframe = false;
		blocks[0].position = iVec2(0);
		blocks[0].size = size;
		blocks[0].hidden = false;
		//blocks[0].gradient = 0.75;

		auto imagepadding = padding;

		//Image
		blocks[1].hidden = true;
		blocks[1].filter = pixmapfilter;
		if (pixmap)
		{
			switch (pixmapalignment)
			{
			//case PIXMAP_TILE:
			//	
			//	break;
			case PIXMAP_CONTAIN:
				aspect = float(size.y) / float(size.x);
				imageaspect = float(pixmap->size.y) / float(pixmap->size.x);
				if (aspect > imageaspect)
				{
					//blocks[1].position.x = padding[0];
					blocks[1].size.x = Min(size.x, pixmap->size.x) - padding[1] - padding[0];
					blocks[1].size.y = Round(float(blocks[1].size.x) * imageaspect);
					//blocks[1].position.y = Round(float(size.y - blocks[1].size.y - padding[2] - padding[3]) / 2.0f);
				}
				else
				{
					//blocks[1].position.y = 0;
					blocks[1].size.y = Min(size.y, pixmap->size.y) - padding[2] - padding[3];
					blocks[1].size.x = Round(float(blocks[1].size.y) / imageaspect);
				}
				blocks[1].position.x = Round(float(size.x - padding[0] - blocks[1].size.x) / 2.0f);
				blocks[1].position.y = Round(float(size.y - padding[2] - blocks[1].size.y) / 2.0f);
				break;
			case PIXMAP_COVER:
				aspect = float(size.y) / float(size.x);
				imageaspect = float(pixmap->size.y) / float(pixmap->size.x);
				if (aspect < imageaspect)
				{
					blocks[1].position.x = 0;
					blocks[1].size.x = size.x;
					blocks[1].size.y = float(size.x) * imageaspect;
					blocks[1].position.y = Round(float(size.y - blocks[1].size.y) / 2.0f);
				}
				else
				{
					blocks[1].position.y = 0;
					blocks[1].size.y = size.y;
					blocks[1].size.x = float(size.y) / imageaspect;
					blocks[1].position.x = Round(float(size.x - blocks[1].size.x) / 2.0f);
				}
				break;
			case PIXMAP_STRETCH:
				blocks[1].position.x = 0;
				blocks[1].position.y = 0;
				blocks[1].size = size;
				break;
			case PIXMAP_CENTER:
				blocks[1].position.x = Round(float(size.x - pixmap->size.x) / 2.0f);
				blocks[1].position.y = Round(float(size.y - pixmap->size.y) / 2.0f);
				blocks[1].size = pixmap->size;
				break;
			default:
				blocks[1].position.x = padding[0];
				blocks[1].position.y = padding[2];
				blocks[1].size.x = pixmap->size.x - padding[1] - padding[0];
				blocks[1].size.y = pixmap->size.y - padding[3] - padding[2];
				break;
			}
			blocks[1].hidden = false;
			blocks[1].color = Vec4(1);
			blocks[1].SetPixmap(pixmap);
		}

		if ((style & PANEL_BORDER) == 0)
		{
			if ((style & PANEL_GROUP) == 0)
			{
				blocks[2].hidden = true;
				blocks[3].hidden = true;
			}
			else
			{
				blocks.resize(5);

				blocks[2].hidden = false;
				blocks[2].color = color[WIDGETCOLOR_FOREGROUND];
				blocks[2].position = iVec2(0) + 8;
				blocks[2].size = size - 16;
				blocks[2].wireframe = true;
				//blocks[2].radius = 5;

				blocks[4].hidden = false;
				blocks[4].color = color[WIDGETCOLOR_FOREGROUND];
				blocks[4].textdefined = true;
				blocks[4].text = text;
				blocks[4].position = iVec2(0) + 8;
				blocks[4].position.x += 8;
				blocks[4].position.y -= gui->GetFontHeight(font, fontscale, fontweight) / 2;
				blocks[4].size.x = gui->GetTextWidth(font, fontscale, text, fontweight);
				blocks[4].size.y = gui->GetFontHeight(font, fontscale, fontweight);
				blocks[4].textalignment = TEXT_LEFT;

				blocks[3].color = color[WIDGETCOLOR_BACKGROUND];
				blocks[3].position = blocks[4].position;
				blocks[3].size = blocks[4].size;
				blocks[3].position.x -= 2;
				blocks[3].size.x += 4;
				blocks[3].hidden = false;
			}
		}
		else
		{
			blocks[2].hidden = false;
			blocks[2].color = color[WIDGETCOLOR_BORDER];
			blocks[2].wireframe = true;
			blocks[2].position = iVec2(0);
			blocks[2].size = size;
		}
		if ((style & PANEL_GROUP) == 0)
		{
			if (!this->text.empty())
			{
				blocks[4].hidden = false;
				blocks[4].size = size;
				blocks[4].SetText(text);
				blocks[4].textalignment = TEXT_CENTER | TEXT_MIDDLE;
				blocks[4].color = color[WIDGETCOLOR_FOREGROUND];
			}
			else
			{
				blocks[4].hidden = true;
				blocks[3].hidden = true;
			}
		}
	}
}