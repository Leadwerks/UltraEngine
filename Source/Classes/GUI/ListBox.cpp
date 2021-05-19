#include "UltraEngine.h"

namespace UltraEngine
{
	ListBox::ListBox()
	{
		classname = "ListBox";
		rebuild = true;
		offset = 0;
	//	color[WIDGETCOLOR_FOREGROUND] = Vec4(1, 1, 1, 1);
	//	color[WIDGETCOLOR_BORDER] = Vec4(0, 0, 0, 1);
	///	color[WIDGETCOLOR_SELECTION] = Vec4(51.f / 255.f * 0.8f, 151.f / 255.f * 0.8f, 1.f);
	//	color[WIDGETCOLOR_BACKGROUND] = Vec4(0.15, 0.15, 0.15, 1);
		//color[WIDGETCOLOR_SELECTION] = Vec4(0.4, 0.4, 0.4, 1);
		blocks.resize(3);
		defaultitemheight = 22;
		textindent = 4;
		sliderwidth = 20;
		//Revert();
	}

	void ListBox::ModifyItem(const int index, const WString& text)
	{
		blocks[1 + index].text = text;
		Widget::ModifyItem(index, text);
	}

	bool ListBox::OK()
	{
		auto item = GetSelectedItem();
		if (item == -1) return false;
		EmitEvent(EVENT_WIDGETACTION, Self(), item);
		return true;
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

	shared_ptr<Widget> CreateListBox(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const ListBoxStyle style)
	{
		auto widget = make_shared<ListBox>();
		parent->kids.push_back(widget);
		widget->parent = parent;
		widget->position.x = x; widget->position.y = y; widget->size.x = width; widget->size.y = height;
		widget->gui = parent->gui;
		float scale = widget->gui->scale;
		widget->edgedistance[0] = x;
		widget->edgedistance[2] = y;
		widget->style = style;
		dynamic_pointer_cast<Widget>(widget)->Initialize("", x, y, width, height, parent, style);
		widget->Invalidate();
		widget->slider = CreateSlider(width - dynamic_pointer_cast<ListBox>(widget)->sliderwidth * scale, 0, dynamic_pointer_cast<ListBox>(widget)->sliderwidth * scale, height - 0, widget, SLIDER_VERTICAL | SLIDER_SCROLLBAR);
		widget->slider->Hide();
		auto engine = GameEngine::Get();
		if (engine) engine->widgets.push_back(widget);
		auto pos = widget->GetPosition(true);
		widget->gui->Redraw(pos.x, pos.y, widget->size.x, widget->size.y);
		return widget;
	}

	/*shared_ptr<Widget> CreateListBox(const int x, const int y, const int width, const int height, shared_ptr<Interface> ui, const ListBoxStyle style)
	{
		auto widget = make_shared<ListBox>();
		ui->rootwidgets.push_back(widget);
		widget->it = ui->rootwidgets.end();
		widget->it--;
		widget->position.x = x; widget->position.y = y; widget->size.x = width; widget->size.y = height;
		widget->gui = ui;
		float scale = widget->gui->scale;
		widget->edgedistance[0] = x;
		widget->edgedistance[2] = y;
		widget->style = style;
		dynamic_pointer_cast<Widget>(widget)->Initialize("", x, y, width, height, ui, style);
		widget->Invalidate();
		widget->slider = CreateSlider(width - dynamic_pointer_cast<ListBox>(widget)->sliderwidth * scale - 1, 1, dynamic_pointer_cast<ListBox>(widget)->sliderwidth * scale, height - 2, widget, SLIDER_VERTICAL | SLIDER_SCROLLBAR);
		auto engine = GameEngine::Get();
		if (engine) engine->widgets.push_back(widget);
		auto pos = widget->GetPosition(true);
		widget->gui->Redraw(pos.x, pos.y, widget->size.x, widget->size.y);
		return widget;
	}*/

	void ListBox::MouseWheel(const int delta, const int x, const int y)
	{
		if (!slider->Hidden())
		{
			slider->MouseWheel(delta, x, y);
			MouseMove(x, y);
		}
	}

	void ListBox::MouseEnter(const int x, const int y)
	{
		hoverstate = true;
		Redraw();
	}

	void ListBox::MouseLeave(const int x, const int y)
	{
		hoverstate = false;
		Redraw();
	}

	int ListBox::ItemHeight()
	{
		return Round(float(gui->GetLineHeight(font, fontscale, fontweight)) * 1.5f);
	}

	void ListBox::MouseMove(const int x, const int y)
	{
		auto* items = &this->items;
		if (filtertext.size() != 0) items = &this->filtereditems;

		auto combobox = this->combobox.lock();
		if (combobox == NULL) return;
		int sw = sliderwidth * gui->scale;
		if (slider->hidestate)
		{
			sw = 0;
		}
		else
		{
			if (slider->knobgrabbed) return;
		}
		if (x > 0 and x < size.x - 1 - sw and y > 1 and y < size.y - 1)
		{
			int item = (y - 1 + offset) / ItemHeight();
			if (item >= 0 and item < items->size())
			{
				if (filtertext.size() != 0)
				{
					int counter = 0;
					for (int n = 0; n < this->items.size(); ++n)
					{
						if (this->items[n].text.Find(filtertext) != -1)
						{
							if (counter == item)
							{
								SelectItem(n);
								break;
							}
							counter++;
						}
					}
				}
				else
				{
					SelectItem(item);
				}
			}
		}
	}

	void ListBox::DoubleClick(const MouseButton button, const int x, const int y)
	{
		if (button == MOUSE_LEFT)
		{
			if (x >= 0 and y >= 0 and x < size.x and y < size.y)
			{
				int item = (y + offset) / ItemHeight();
				if (item >= 0 and item < items.size())
				{
					EmitEvent(EVENT_WIDGETACTION, As<Widget>(), item);
				}
			}
		}
	}

	void ListBox::MouseDown(const MouseButton button, const int x, const int y)
	{
		if (button == MOUSE_LEFT or button == MOUSE_RIGHT)
		{
			if (x >= 0 and y >= 0 and x < size.x and y < size.y)
			{
				int item = (y + offset) / ItemHeight();
				if (item >= 0 and item < items.size())
				{
					if (button == MOUSE_LEFT)
					{
						auto combobox = this->combobox.lock();						
						if (filtertext.size() != 0)
						{
							int counter = 0;
							for (int n = 0; n < this->items.size(); ++n)
							{
								if (this->items[n].text.Find(filtertext) != -1)
								{
									if (counter == item)
									{
										if (GetSelectedItem() != n)
										{
											SelectItem(n);
											EmitEvent(EVENT_WIDGETSELECT, dynamic_pointer_cast<Widget>(Self()), n);
										}
										if (combobox) combobox->SelectItem(n);
										break;
									}
									counter++;
								}
							}
						}
						else
						{
							if (GetSelectedItem() != item)
							{
								SelectItem(item);
								EmitEvent(EVENT_WIDGETSELECT, Self(), item);
							}
							if (combobox)
							{
								if (combobox->GetSelectedItem() != item)
								{
									combobox->SelectItem(item);
									EmitEvent(EVENT_WIDGETSELECT, combobox, item);
								}
							}
						}
						if (combobox) combobox->Close();
					}
					else
					{
						EmitEvent(EVENT_WIDGETMENU, dynamic_pointer_cast<Widget>(Self()), item);
					}
				}
			}
		}
	}

	void ListBox::MouseUp(const MouseButton button, const int x, const int y)
	{
	}

	void ListBox::GainFocus()
	{
	}

	void ListBox::LoseFocus()
	{
		//auto combobox = this->combobox.lock();
		//if (combobox)
		//{
		//	combobox->Close();
		//}
	}

	void ListBox::KeyDown(const KeyCode key)
	{
		auto* items = &this->items;
		if (filtertext.size() != 0) items = &filtereditems;

		auto combobox = this->combobox.lock();
		if (combobox)
		{
			return;
		}
		if (key == KEY_RIGHT or key == KEY_DOWN)
		{
			int item = GetSelectedItem() + 1;
			if (item < items->size())
			{
				SelectItem(item);
				EmitEvent(EVENT_WIDGETSELECT, Self(), item);
				//auto combobox = this->combobox.lock();
				//if (combobox) combobox->SelectItem(item);
				//EmitEvent(EVENT_WIDGET_SELECT, Self(), item);
			}
		}
		else if (key == KEY_LEFT or key == KEY_UP)
		{
			int item = GetSelectedItem() - 1;
			if (item > -1)
			{
				SelectItem(item);
				EmitEvent(EVENT_WIDGETSELECT,Self(),item);
				//auto combobox = this->combobox.lock();
				//if (combobox) combobox->SelectItem(item);
				//EmitEvent(EVENT_WIDGET_SELECT, Self(), item);
			}
		}
		Widget::KeyDown(key);
	}

	void ListBox::SelectItem(const int index)
	{
		auto* items = &this->items;
		if (filtertext.size() != 0) items = &filtereditems;
		if (index < -1 or index >= items->size()) return;
		if (index >= 0)
		{
			if (1 + (index) *ItemHeight() - offset < 0)
			{
				offset = 1 + (index) *ItemHeight();
			}
			else if (1 + (index + 1) * ItemHeight() - offset > size.y)
			{
				offset = 1 + (index + 1) * ItemHeight() - size.y;
			}
			if (!slider->Hidden()) slider->SetValue(offset / ItemHeight());
		}
		Widget::SelectItem(index);
	}

	void ListBox::SetShape(int x, int y, int width, int height)
	{
		if (x != position.x or y != position.y or width != size.x or height != size.y)
		{
			Widget::SetShape(x, y, width, height);
			UpdateSlider();
		}
	}

	void ListBox::UpdateLayout()
	{
		Widget::UpdateLayout();
		UpdateSlider();
	}

	bool ListBox::ProcessEvent(const Event& e)
	{
		if (e.source == slider)
		{
			if (e.id == EVENT_WIDGETACTION)
			{
				offset = e.data * ItemHeight();
				Redraw();
			}
			else
			{
				if (e.id == EVENT_WIDGETGAINFOCUS)
				{
				//	Activate();		
				}
			}
			return false;
		}
		return true;
	}

	void ListBox::UpdateSlider()
	{
		int height = Ceil(float(size.y) / float(ItemHeight())) * ItemHeight();

		int visibleitems = Round( float(size.y) / float(ItemHeight()) );

		auto* items = &this->items;
		if (filtertext.size() != 0) items = &filtereditems;

		if (items->size() * ItemHeight() > GetSize().y)
		{
			slider->SetShape(size.x - sliderwidth * gui->scale - 0, 0, sliderwidth * gui->scale, size.y - 0);
			slider->Show();
			slider->SetRange(visibleitems, items->size());
			//slider->SetRange(size.y, items->size() * ItemHeight());
			//slider->SetRange(1, Round( float((items.size() * ItemHeight() + 2 - height)) / float(itemheight) ) );
			slider->SetValue(Round(float(offset) / float(ItemHeight())));
		}
		else
		{
			slider->Hide();
			offset = 0;
		}
	}

	void ListBox::Reset()
	{
		//bool slidervisible = !slider->Hidden();
		int sw = 0;// sliderwidth;
		//if (!slidervisible) sw = 0;

		blocks.clear();

		//Backgound
		AddBlock(iVec2(0, 0), iVec2(size.x - sw, size.y), color[WIDGETCOLOR_BACKGROUND]);

		//Selection
		AddBlock(iVec2(0, -offset), iVec2(size.x - sw, ItemHeight()), color[WIDGETCOLOR_SELECTION]);
		int item = GetSelectedItem();
		if (item == -1) blocks[1].hidden = true;
		blocks[1].position.y = item * ItemHeight() - offset;

		//Border
		AddBlock(iVec2(0, 0), iVec2(size.x - sw, size.y), color[WIDGETCOLOR_BORDER], true);
	}

	int ListBox::AddItem(const WString& text, const bool selected)
	{
		rebuild = true;
		int n = Widget::AddItem(text, selected);
		int sw = sliderwidth;
		//auto borderblock = blocks[blocks.size() - 1];
		//blocks.resize(blocks.size() - 1);
		//AddBlock(text, iVec2(textindent, 0), iVec2(size.x - textindent - sw, ItemHeight()), color[WIDGETCOLOR_FOREGROUND], TEXT_LEFT | TEXT_MIDDLE);
		//blocks.push_back(borderblock);
		UpdateSlider();
		return n;
	}

	void ListBox::FilterItems(const WString& filter)
	{
		Lock();
		Widget::FilterItems(filter);
		UpdateSlider();
		Unlock();
	}

	void ListBox::ClearItems()
	{
		rebuild = true;
		Widget::ClearItems();
		auto borderblock = blocks[blocks.size() - 1];
		blocks.resize(2);
		blocks.push_back(borderblock);
		UpdateSlider();
	}

	void ListBox::Draw(const int x, const int y, const int width, const int height)
	{
		blocks[0].color = color[WIDGETCOLOR_SUNKEN];
		blocks[1].color = color[WIDGETCOLOR_SELECTION];

		if (rebuild)
		{
			float scale = gui->scale;

			bool slidervisible = !slider->Hidden();
			int sw = sliderwidth * gui->scale;
			if (!slidervisible) sw = 0;

			//Background
			blocks[0].size.x = size.x - sw;
			blocks[0].size.y = size.y;
			
			//Selection
			int item = GetSelectedItem();
			blocks[1].hidden = item == -1;
			blocks[1].position.y = item * ItemHeight() - offset;
			blocks[1].size.x = size.x - sw;
			blocks[1].size.y = ItemHeight();
			if (!filtertext.empty()) blocks[1].hidden = true;

			//Items
			auto items = this->items;
			if (!filtertext.empty()) items = filtereditems;

			float indent = 4;

			blocks.resize(items.size() + 3);
			int iy = 1 - offset;			
			for (int n = 0; n < items.size(); ++n)
			{
				//if (items[n].index)
				//AddBlock(text, iVec2(textindent, 0), iVec2(size.x - textindent - sw, ItemHeight()), color[WIDGETCOLOR_FOREGROUND], TEXT_LEFT | TEXT_MIDDLE);
				blocks[2 + n].color = color[WIDGETCOLOR_FOREGROUND];
				if (n == item) blocks[2 + n].color = color[WIDGETCOLOR_SELECTEDTEXT];
				blocks[2 + n].textalignment = TEXT_LEFT | TEXT_MIDDLE;
				blocks[2 + n].position.x = Round(indent * scale);
				blocks[2 + n].SetText(items.at(n).text);
				blocks[2 + n].position.y = iy;
				blocks[2 + n].size.x = size.x - sw;
				blocks[2 + n].size.y = ItemHeight();// *scale;
				blocks[2 + n].hidden = false;
				iy += ItemHeight();// *scale;
			}

			//Border
			blocks[blocks.size() - 1].hidden = false;
			blocks[blocks.size() - 1].color = color[WIDGETCOLOR_BORDER];
			blocks[blocks.size() - 1].wireframe = true;
			blocks[blocks.size() - 1].size.x = size.x;
			blocks[blocks.size() - 1].size.y = size.y;
			if (!slider->Hidden(false))
			{
				blocks[blocks.size() - 1].size.x = slider->position.x;
			}
		}
	}
}