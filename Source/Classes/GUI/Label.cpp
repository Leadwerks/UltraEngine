#include "UltraEngine.h"

namespace UltraEngine
{
	Label::Label()
	{
		this->classname = "Label";
		color[WIDGETCOLOR_FOREGROUND] = Vec4(1, 1, 1, 1);
		blocks.resize(1);
	}

	shared_ptr<Widget> CreateLabel(const WString& text, const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const LabelStyle style)
	{
		auto widget = make_shared<Label>();
		if (!widget->Initialize(text, x, y, width, height, parent, style)) return NULL;
		widget->Invalidate();
		auto pos = widget->GetPosition(true);
		widget->gui->Redraw(pos.x, pos.y, widget->size.x, widget->size.y);
		return widget;
	}

	/*shared_ptr<Widget> CreateLabel(const WString& text, const int x, const int y, const int width, const int height, shared_ptr<Interface> ui, const LabelStyle style)
	{
		auto widget = make_shared<Label>();
		if (!widget->Initialize(text, x, y, width, height, ui, style)) return NULL;
		widget->Invalidate();
		auto pos = widget->GetPosition(true);
		widget->gui->Redraw(pos.x, pos.y, widget->size.x, widget->size.y);
		return widget;
	}*/

	void Label::Draw(const int x, const int y, const int width, const int height)
	{
		if (text.length() == 0)
		{
			blocks[0].hidden = true;
			if ((LABEL_BORDER & style) == 0) return;
		}
		else
		{
			blocks[0].hidden = false;
			blocks[0].color = color[WIDGETCOLOR_FOREGROUND];
			blocks[0].position = iVec2(0);
			blocks[0].size = size;
			blocks[0].textdefined = true;
			blocks[0].text = text;
			blocks[0].textalignment = style;
		}

		if ((LABEL_BORDER & style) != 0)
		{
			blocks[0].position.x = 1;
			blocks[0].position.y = 1;
			blocks[0].size.x -= 2;
			blocks[0].size.y -= 2;

			blocks.resize(2);
			blocks[1].hidden = false;
			blocks[1].wireframe = true;
			blocks[1].color = color[WIDGETCOLOR_FOREGROUND];
			blocks[1].size = size;
		}
	}
}