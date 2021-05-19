#include "UltraEngine.h"

namespace UltraEngine
{
	Button::Button()
	{
		classname = "Button";
		checkboxsize = 16;
		hoverstate = false;
		pushstate = false;
		//color[WIDGETCOLOR_FOREGROUND] = Vec4(1, 1, 1, 1);
		//color[WIDGETCOLOR_BORDER] = Vec4(0, 0, 0, 1);
		//color[WIDGETCOLOR_HIGHLIGHT] = Vec4(51.f / 255.f * 0.8f, 151.f / 255.f * 0.8f, 1.f);
		//color[WIDGETCOLOR_BACKGROUND] = Vec4(0.20, 0.20, 0.20, 1);
		cornerradius = 8;
		blocks.resize(3);
		//Revert();
	}

	/*void Button::Revert()
	{
		color[WIDGETCOLOR_FOREGROUND] = Vec4(1, 1, 1, 1);
		color[WIDGETCOLOR_BORDER] = Vec4(0, 0, 0, 1);
		color[WIDGETCOLOR_SELECTION] = Vec4(51 / 255, 151 / 255, 1);
		color[WIDGETCOLOR_BACKGROUND_TOP] = Vec4(0.20, 0.20, 0.20, 1);
		color[WIDGETCOLOR_BACKGROUND_BOTTOM] = Vec4(0.15, 0.15, 0.15, 1);
		cornerradius = 8;
	}*/

	/*void Button::ApplyTheme(const nlohmann::json& j3)
	{
		if (!j3["theme"].is_object()) return;
		if (!j3["theme"]["button"].is_object()) return;
		if (j3["theme"]["button"]["color"].is_object())
		{
			if (j3["theme"]["button"]["color"]["foreground"].is_array())
			{
				if (j3["theme"]["button"]["color"]["foreground"].size() == 4)
				{
					color[WIDGETCOLOR_FOREGROUND].r = j3["theme"]["button"]["color"]["foreground"][0];
					color[WIDGETCOLOR_FOREGROUND].g = j3["theme"]["button"]["color"]["foreground"][1];
					color[WIDGETCOLOR_FOREGROUND].b = j3["theme"]["button"]["color"]["foreground"][2];
					color[WIDGETCOLOR_FOREGROUND].a = j3["theme"]["button"]["color"]["foreground"][3];
				}
			}
		}
	}*/

	void Button::KeyDown(const KeyCode key)
	{ 
		/*if (key == KEY_ENTER)
		{
			if (!gui->MouseDown(MOUSE_LEFT))
			{
				MouseDown(MOUSE_LEFT, 0, 0);
				MouseUp(MOUSE_LEFT, 0, 0);
			}
		}*/
		Widget::KeyDown(key);
	}

	void Button::Reset()
	{
		blocks.resize(4);
		auto scale = gui->scale;
		if ((style & BUTTON_CHECKBOX) != 0 or (style & BUTTON_RADIO) != 0)
		{
			color[WIDGETCOLOR_BACKGROUND] = Vec4(0.15, 0.15, 0.15, 1);
			//auto pm = ;//LoadPixmap("Materials//UI/button_checkmark.svg");
			//if (pm == NULL) pm = CreatePixmap(16, 16);
			if ((style & BUTTON_RADIO) == 0)
			{
				AddBlock(gui->icon_button_checkmark, iVec2(2 + (Round(float(checkboxsize) * scale) - gui->icon_button_checkmark->size.x) / 2, 2 + (Round(float(checkboxsize) * scale) - gui->icon_button_checkmark->size.y) / 2), 0.8);//, color[WIDGETCOLOR_FOREGROUND]);
			}
		}
		blocks.resize(6);
		Widget::Reset();
	}

	shared_ptr<Widget> CreateButton(const WString& text, const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const ButtonStyle style)
	{
		auto widget = make_shared<Button>();
		widget->blocks.resize(6);
		if (!widget->Initialize(text, x, y, width, height, parent, style)) return NULL;
		//dynamic_pointer_cast<Widget>(widget)->Initialize(text, x, y, width, height, parent, style);
		if ((BUTTON_OK & style) != 0) widget->sensitivity = WIDGET_OK;
		if ((BUTTON_CANCEL & style) != 0) widget->sensitivity = WIDGET_CANCEL;
		widget->Invalidate();
		auto pos = widget->GetPosition(true);
		widget->gui->Redraw(pos.x, pos.y, widget->size.x, widget->size.y);
		return widget;
	}

	/*shared_ptr<Widget> CreateButton(const WString& text, const int x, const int y, const int width, const int height, shared_ptr<Interface> ui, const ButtonStyle style)
	{
		auto widget = make_shared<Button>();
		dynamic_pointer_cast<Widget>(widget)->Initialize(text, x, y, width, height, ui, style);
		//ui->rootwidgets.push_back(widget);
		//widget->position.x = x; widget->position.y = y; widget->size.x = width; widget->size.y = height;
		//widget->gui = ui;
		//widget->edgedistance[0] = x;
		//widget->edgedistance[2] = y;
		//widget->style = style;
		if ((BUTTON_OK & style) != 0) widget->sensitivity = WIDGET_OK;
		if ((BUTTON_CANCEL & style) != 0) widget->sensitivity = WIDGET_CANCEL;
		if (!widget->Initialize(text, x, y, width, height, ui, style)) return NULL;
		widget->Invalidate();
		auto pos = widget->GetPosition(true);
		widget->gui->Redraw(pos.x, pos.y, widget->size.x, widget->size.y);
		return widget;
	}*/

	void Button::SetState(const WidgetState state)
	{
		if ((style & BUTTON_RADIO) != 0)
		{
			if (state == WIDGETSTATE_SELECTED)
			{
				auto next = GetNext();
				while (next)
				{
					if (dynamic_pointer_cast<Button>(next))
					{
						if ((next->style & BUTTON_RADIO) != 0)
						{
							if (next->state != WIDGETSTATE_UNSELECTED)
							{
								next->state = WIDGETSTATE_UNSELECTED;
								//EmitEvent(EVENT_WIDGETACTION, next, 0);
								next->Redraw();
							}
							next = next->GetNext();
							continue;
						}
					}
					break;
				}
				next = GetPrev();
				while (next)
				{
					if (dynamic_pointer_cast<Button>(next))
					{
						if ((next->style & BUTTON_RADIO) != 0)
						{
							if (next->state != WIDGETSTATE_UNSELECTED)
							{
								next->state = WIDGETSTATE_UNSELECTED;
								//EmitEvent(EVENT_WIDGETACTION, next, 0);
								next->Redraw();
							}
							next = next->GetPrev();
							continue;
						}
					}
					break;
				}
			}
		}
		Widget::SetState(state);
	}

	void Button::MouseEnter(const int x, const int y)
	{
		Widget::MouseEnter(x, y);
		if (!gui->MouseDown(MOUSE_LEFT))
		{
			hoverstate = true;
			Redraw();
		}
	}

	void Button::MouseLeave(const int x, const int y)
	{
		Widget::MouseLeave(x, y);
		hoverstate = false;
		Redraw();
	}

	void Button::MouseDown(const MouseButton button, const int x, const int y)
	{
		if (button != MOUSE_LEFT) return;
		if (hoverstate == 0) return;
		pushstate = true;
		Redraw();
	}

	void Button::MouseUp(const MouseButton button, const int x, const int y)
	{
		if (pushstate == true)
		{
			pushstate = false;
			if ((style & BUTTON_RADIO) != 0)
			{
				if (!GetState())
				{
					SetState(WIDGETSTATE_SELECTED);
					EmitEvent(EVENT_WIDGETACTION, Self(), 1);
				}
			}
			else
			{
				if ((style & BUTTON_TOGGLE) != 0 or (style & BUTTON_CHECKBOX) != 0 or (style & BUTTON_RADIO) != 0)
				{
					if (GetState() == WIDGETSTATE_SELECTED)
					{
						SetState(WIDGETSTATE_UNSELECTED);
					}
					else
					{
						SetState(WIDGETSTATE_SELECTED);
					}
					EmitEvent(EVENT_WIDGETACTION, Self(), GetState());
				}
			}
			Redraw();
			if ((style & BUTTON_RADIO) == 0 and (style & BUTTON_CHECKBOX) == 0)
			{
				if (x >= 0 and y >= 0 and x < size.x and y < size.y)
				{
					EmitEvent(EVENT_WIDGETACTION, Self(), 0, 0, 0, 0, 0);
				}
			}
		}
		MouseMove(x, y);
	}

	void Button::MouseMove(const int x, const int y)
	{
		if (x > 0 and y > 0 and x < size.x and y < size.y)
		{
			MouseEnter(x, y);
		}
	}

	void Button::LoseFocus()
	{
		hoverstate = false;
		pushstate = false;
		Redraw();
	}

	void Button::Draw(const int x, const int y, const int width, const int height)
	{
		float scale = gui->scale;

		if ((style & BUTTON_CHECKBOX) != 0 or (style & BUTTON_RADIO) != 0)
		{
			//Checkbox background
			blocks[0].color = color[WIDGETCOLOR_SUNKEN];
			if (pushstate) blocks[0].color = color[WIDGETCOLOR_SELECTION] * 0.25 + color[WIDGETCOLOR_SUNKEN] * 0.75;
			blocks[0].position.x = 2;
			blocks[0].position.y = 2;
			blocks[0].size.x = Round(float(checkboxsize) * scale);
			blocks[0].size.y = Round(float(checkboxsize) * scale);
			blocks[0].gradient = 1.0f;
			blocks[0].hidden = false;
			if ((style & BUTTON_RADIO) != 0) blocks[0].radius = float(blocks[0].size.x) / 2.0f;

			//Icon
			blocks[2].hidden = true;
			
			//Checkbox border
			blocks[3].color = color[WIDGETCOLOR_BORDER];
			if (pushstate == true or hoverstate == true) blocks[3].color = color[WIDGETCOLOR_SELECTION];
			blocks[3].position.x = 2;
			blocks[3].position.y = 2;
			blocks[3].size.x = Round(float(checkboxsize) * scale);
			blocks[3].size.y = Round(float(checkboxsize) * scale);
			blocks[3].wireframe = true;
			blocks[3].hidden = false;
			if ((style & BUTTON_RADIO) != 0) blocks[3].radius = float(blocks[3].size.x) / 2.0f;

			//Label
			blocks[1].color = color[WIDGETCOLOR_FOREGROUND];
			blocks[1].position.x = blocks[0].position.x + blocks[0].size.x + Round(8.0f * scale);
			blocks[1].position.y = blocks[0].position.y;
			blocks[1].textdefined = true;
			blocks[1].size = size - blocks[1].position;
			blocks[1].size.y = blocks[0].size.y;
			blocks[1].SetText(text);
			blocks[1].textalignment = TEXT_LEFT | TEXT_MIDDLE;
			blocks[1].hidden = false;

			//Checkbox
			blocks[4].hidden = true;
			blocks[4].color = color[WIDGETCOLOR_FOREGROUND];
			if ((style & BUTTON_RADIO) == 0)
			{
				blocks[4].hidden = !GetState();		
				//blocks[4].SetPixmap(gui->icon_button_checkmark);
				blocks[4].position.x = Round(2.0f * gui->scale) + (blocks[3].size.x - blocks[4].size.x) / 2;
				blocks[4].position.y = Round(2.0f * gui->scale) + (blocks[3].size.y - blocks[4].size.y) / 2;
				if (GetState() == WIDGETSTATE_INDETERMINATE)
				{
					blocks[4].color.a = 0.5f;
				}
				else
				{
					blocks[4].color.a = 1.0f;
				}
				//blocks[4].position = iVec2(2 + (Round(float(checkboxsize) * scale) ) / 2 - blocks[4].size / 2;
				//blocks[4].size = gui->icon_button_checkmark->size;
			}

			if ((style & BUTTON_RADIO) != 0)
			{
				blocks[5].hidden = true;
				if (GetState())
				{
					blocks[5].size.x = blocks[3].size.x * 0.4;
					blocks[5].size.y = blocks[3].size.y * 0.4;
					blocks[5].position.x = blocks[3].position.x + (blocks[3].size.x - blocks[5].size.x) / 2;
					blocks[5].position.y = blocks[3].position.y + (blocks[3].size.y - blocks[5].size.y) / 2;
					blocks[5].radius = float(blocks[5].size.x) * 0.5f;
					blocks[5].hidden = false;
					blocks[5].color = color[WIDGETCOLOR_FOREGROUND];
				}
			}
		}
		else
		{
			//Background
			blocks[0].color = color[WIDGETCOLOR_BACKGROUND];
			if ((style & BUTTON_OK) != 0 and (BUTTON_TOOLBAR & style) == 0)
			{
				blocks[0].color = color[WIDGETCOLOR_SELECTION];
			}
			if (hoverstate) blocks[0].color += 0.05;
			blocks[0].gradient = 0.75f;
			blocks[0].position = iVec2(0);
			blocks[0].size = size;
			blocks[0].radius = cornerradius;
			blocks[0].hidden = false;
			if (pushstate == true or GetState() == true)
			{
				blocks[0].color = color[WIDGETCOLOR_BACKGROUND] * 0.75f;
				if ((style & BUTTON_OK) != 0)
				{
					blocks[0].color = color[WIDGETCOLOR_SELECTION] * 0.75f;
				}
				blocks[0].gradient = 1.0f / 0.75f;
			}

			//Toolbar button style
			if ((BUTTON_TOOLBAR & style) != 0)
			{
				blocks[0].gradient = 1.0f;
				if (pushstate == true or GetState() == true)// or hoverstate == true)
				{
					blocks[0].color = color[WIDGETCOLOR_SELECTION] * 0.25 + color[WIDGETCOLOR_BACKGROUND] * 0.75;
				}
				else
				{
					if (hoverstate or GetState() == true)
					{
						blocks[0].color = color[WIDGETCOLOR_HIGHLIGHT];
					}
					else
					{
						blocks[0].color = color[WIDGETCOLOR_BACKGROUND];
					}
				}
			}

			//Label
			/*if ((BUTTON_TOOLBAR & style) != 0)
			{
				blocks[1].hidden = false;
			}
			else*/
			{
				blocks[1].hidden = false;
				blocks[1].color = color[WIDGETCOLOR_FOREGROUND];
				blocks[1].position = iVec2(0);
				blocks[1].textdefined = true;
				blocks[1].size = size;
				blocks[1].SetText(text);
				blocks[1].textalignment = TEXT_CENTER | TEXT_MIDDLE;
				if (pushstate == true or GetState() == true)
				{
					blocks[1].position.x++;
					blocks[1].position.y++;
					//blocks[1].size.x--;
					//blocks[1].size.y--;
				}
				if ((style & BUTTON_OK) != 0)// and (BUTTON_TOOLBAR & style) == 0)
				{
					blocks[1].color = color[WIDGETCOLOR_SELECTEDTEXT];
				}
			}

			//Icon
			blocks[2].hidden = true;
			if (pixmap)
			{
				blocks[2].SetPixmap(pixmap);
				blocks[2].filter = pixmapfilter;
				blocks[2].hidden = false;
				blocks[2].color = gui->colors[WIDGETCOLOR_FOREGROUND];
				if (text == L"")
				{
					blocks[2].position.x = Round(float(size.x - pixmap->size.x) / 2.0f);
				}
				else
				{
					blocks[2].position.x = Round(float(size.x - gui->GetTextWidth(font, fontscale, text, fontweight)) / 2.0f) - pixmap->size.x - Round(4.0f * scale);
				}
				blocks[2].position.y = Round(float(size.y - pixmap->size.y) / 2.0f);
				if (pushstate == true or GetState() == true)
				{
					if ((BUTTON_TOOLBAR & style) == 0)
					{
						blocks[2].position.x++;
						blocks[2].position.y++;
					}
				}
				blocks[2].size = pixmap->size;
			}

			//Border
			if ((BUTTON_TOOLBAR & style) == 0)
			{
				if (hoverstate == true or (style & BUTTON_OK) != 0)
				{
					blocks[3].color = color[WIDGETCOLOR_SELECTION];
				}
				else
				{
					blocks[3].color = color[WIDGETCOLOR_BORDER];
				}
			}
			else
			{
				if (pushstate == true or GetState() == true)
				{
					blocks[3].color = color[WIDGETCOLOR_SELECTION];
				}
				else
				{
					blocks[3].color = color[WIDGETCOLOR_BORDER];
				}
			}
			blocks[3].hidden = false;
			blocks[3].position = iVec2(0);
			blocks[3].size = size;
			blocks[3].wireframe = true;
			blocks[3].radius = cornerradius;
			if ((BUTTON_TOOLBAR & style) != 0)
			{
				blocks[3].radius = 0.0f;
				blocks[0].radius = 0.0f;
			}
		} 
	}
}