#include "UltraEngine.h"

namespace UltraEngine
{
	TextField::TextField()
	{
		ctrlpressed = false;
		classname = "TextField";
		doubleclickrange = 1;
		pressed = false;
		offsetx = 0;
		x = 0;
		sellen = 0;
		caretposition = 0;
		textindent = 4;
		hoverstate = false;
		cursorblinkstate = true;
		shiftpressed = false;
		pushstate = false;
		//color[WIDGETCOLOR_FOREGROUND] = Vec4(1, 1, 1, 1);
		//color[WIDGETCOLOR_BORDER] = Vec4(0, 0, 0, 1);
		//color[WIDGETCOLOR_SELECTION] = Vec4(0.4, 0.4, 0.4, 1);
		//color[WIDGETCOLOR_BACKGROUND] = Vec4(0.15, 0.15, 0.15, 1);
		blocks.resize(5);
		//Revert();
	}

	/*void Button::Revert()
	{
		color[WIDGETCOLOR_FOREGROUND] = Vec4(1, 1, 1, 1);
		color[WIDGETCOLOR_BORDER] = Vec4(0, 0, 0, 1);
		color[WIDGETCOLOR_SELECTION] = Vec4(0.4, 0.4, 0.4, 1);
		color[WIDGETCOLOR_BACKGROUND] = Vec4(0.15, 0.15, 0.15, 1);
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

	bool TextField::OK()
	{
		KeyDown(KEY_ENTER);
		return true;
	}

	void TextField::CursorBlink()
	{
		cursorblinkstate = !cursorblinkstate;
		if (gui->GetFocus() == Self()) Redraw();
	}

	int TextField::GetCharAtPosition(const int pos, const bool clickonchar)
	{
		auto text = this->text;
		if ((style & TEXTFIELD_PASSWORD) != 0) text = wstring(this->text.size(), L'•');

		auto scale = gui->GetScale();
		float x = Round(scale * float(textindent));
		int count = text.length();
		int lastcharwidth = 0;
		for (int n = 0; n < count; ++n)
		{
			auto c = text.Mid(n, 1);
			lastcharwidth = gui->GetTextWidth(font, fontscale, c, fontweight);
			if (clickonchar)
			{
				if (x >= pos) return n;
			}
			else
			{
				if (x >= pos - lastcharwidth / 2) return n;
			}
			x += lastcharwidth;
		}
		return count;
	}

	int TextField::GetCaretCoord()
	{
		return GetCaretCoord(caretposition);
	}

	int TextField::GetCaretCoord(const int caret)
	{
		auto text = this->text;
		if ((style & TEXTFIELD_PASSWORD) != 0) text = wstring(this->text.size(), L'•');
		int x = Round(gui->GetScale() * float(textindent));
		int count = Min(caret, text.length());
		for (int n = 0; n < count; ++n)
		{
			auto c = text.Mid(n, 1);
			x += gui->GetTextWidth(font, fontscale, c, fontweight);
		}
		return x;
	}

	shared_ptr<Widget> CreateTextField(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const TextFieldStyle style)
	{
		auto widget = make_shared<TextField>();
		parent->kids.push_back(widget);
		widget->parent = parent;
		widget->position.x = x; widget->position.y = y; widget->size.x = width; widget->size.y = height;
		widget->gui = parent->gui;
		widget->edgedistance[0] = x;
		widget->edgedistance[2] = y;
		widget->style = style;
		if ((style & TEXTFIELD_READONLY) == 0) widget->SetPointer(POINTER_IBEAM);
		widget->Invalidate();
		auto pos = widget->GetPosition(true);
		widget->gui->Redraw(pos.x, pos.y, widget->size.x, widget->size.y);
		return widget;
	}

	/*shared_ptr<Widget> CreateTextField(const int x, const int y, const int width, const int height, shared_ptr<Interface> ui, const TextFieldStyle style)
	{
		auto widget = make_shared<TextField>();
		ui->rootwidgets.push_back(widget);
		widget->it = ui->rootwidgets.end();
		widget->it--;
		widget->position.x = x; widget->position.y = y; widget->size.x = width; widget->size.y = height;
		widget->gui = ui;
		widget->edgedistance[0] = x;
		widget->edgedistance[2] = y;
		widget->style = style;
		if ((style & TEXTFIELD_READONLY) == 0) widget->SetPointer(POINTER_IBEAM);
		widget->Invalidate();
		auto pos = widget->GetPosition(true);
		widget->gui->Redraw(pos.x, pos.y, widget->size.x, widget->size.y);
		return widget;
	}*/

	void TextField::MouseDown(const MouseButton button, const int x_, const int y)
	{
		//if ((style & TEXTFIELD_READONLY) != 0) return;

		lastmouseposition.x = x;
		lastmouseposition.y = y;
		int x = x_;
		if (button == MOUSE_LEFT)
		{
			x -= offsetx;
			lastmouseposition.x = x;
			lastmouseposition.y = y;

			//Position caret under mouse click
			int prevcaretposition = caretposition + sellen;
			//cursorblinkmode = true;
			caretposition = GetCharAtPosition(x,false);
			//cursorblinkmode = true;
			pressed = true;
			if (shiftpressed)
			{
				sellen = prevcaretposition - caretposition;
			}
			else
			{
				sellen = 0;
				cursorblinkstate = true;
				gui->ResetCursorBlink();
			}
			Redraw();
		}
		else if (button == MOUSE_RIGHT)
		{
			EmitEvent(EVENT_WIDGETMENU, Self(), 0, x, y);
		}
	}

	void TextField::DoubleClick(const MouseButton button, const int x_, const int y)
	{
		//if ((style & TEXTFIELD_READONLY) != 0) return;
		
		if (button != MOUSE_LEFT) return;
		int x = x_ - offsetx;
		if (Abs(lastmouseposition.x - x) <= doubleclickrange and Abs(lastmouseposition.y - y) <= doubleclickrange)
		{
			WString badchars = "<>,./\\?[]{}!@#$%^&*()-_=+| 	";

			//Select the word at the mouse position
			auto text = this->text;
			if ((style & TEXTFIELD_PASSWORD) != 0) text = wstring(this->text.size(), L'•');

			int l = text.length();
			int c = GetCharAtPosition(x, true);
			caretposition = c;
			sellen = -1;
			auto chr = text.Mid(c - 1, 1);

			if (chr == L" " or chr == L"	")
			{
				//Select spaces in this word before the clicked character
				for (int n = c - 2; n >= 0; n--)
				{
					Print(text.Mid(n, 1));
					if (text.Mid(n, 1) != L" " and text.Mid(n, 1) != L"	")
					{
						break;
					}
					else
					{
						sellen--;
					}
				}

				//Select spaces in this word after the clicked character
				for (int n = c; n < l; n++)
				{
					if (text.Mid(n, 1) != L" " and text.Mid(n, 1) != L"	")
					{
						break;
					}
					else
					{
						caretposition++;
						sellen--;
					}
				}
			}
			else if (badchars.Find(chr) > -1)
			{
				//Stop here
			}
			else
			{
				//Select characters in this word before the clicked character
				for (int n = c - 2; n >= 0; n--)
				{
					chr = text.Mid(n, 1);
					if (badchars.Find(chr) > -1)
					{
						break;
					}
					else
					{
						sellen--;
					}
				}

				//Select characters in this word after the clicked character
				for (int n = c; n < l; n++)
				{
					chr = text.Mid(n, 1);
					if (badchars.Find(chr) > -1)
					{
						break;
					}
					else
					{
						caretposition++;
						sellen--;
					}
				}
			}
			//self.gui:ResetCursorBlink()
			//cursorblinkmode = true
			pressed = false;
			Redraw();
		}
	}
	
	void TextField::TripleClick(const MouseButton button, const int x, const int y)
	{
		//if ((style & TEXTFIELD_READONLY) != 0) return;
		if (button != MOUSE_LEFT) return;
		int l = text.length();
		caretposition = l;
		sellen = -int(text.size());
		Redraw();
	}

	void TextField::MouseUp(const MouseButton button, const int x, const int y)
	{
		if (button == MOUSE_LEFT) pressed = false;
	}
	
	void TextField::MouseMove(const int x_, const int y)
	{
		int x = x_;
		if (pressed)
		{
			//Select range of characters
			x = x - offsetx;
			int currentcaretpos = caretposition;
			int prevcaretpos = caretposition + sellen;
			//cursorblinkmode = true;
			caretposition = GetCharAtPosition(x,false);
			int oldsellen = sellen;
			if (caretposition != currentcaretpos)
			{
				cursorblinkstate = true;
				gui->ResetCursorBlink();
				sellen = prevcaretpos - caretposition;
				Redraw();
			}
			UpdateOffset();
		}
		Widget::MouseMove(x_, y);
	}
	
	void TextField::UpdateOffset()
	{
		int width = GetSize().x;
		float scale = gui->GetScale();
		auto c = text.Right(1);
		int cw = gui->GetTextWidth(font, fontscale, c, fontweight);
		int tw = gui->GetTextWidth(font, fontscale, text, fontweight);
		if (tw + scale * textindent * 2 > width)
		{
			auto fragment = GetSelectedText();
			int fw = gui->GetTextWidth(font, fontscale, fragment, fontweight);
			if (fw + scale * textindent * 2 > width)
			{
				int coord = GetCaretCoord();
				if (offsetx + coord - scale * textindent < 0)
				{
					offsetx = -coord + scale * textindent;
				}
				else if (offsetx + coord > width - scale * textindent)
				{
					offsetx = -(coord - (width - scale * textindent));
				}
			}
			else
			{
				int c1 = Min(caretposition, caretposition + sellen);
				int c2 = Max(caretposition, caretposition + sellen);
				int coord1 = GetCaretCoord(c1);
				int coord2 = GetCaretCoord(c2);
				if (offsetx + coord1 - scale * textindent < 0)
				{
					offsetx = -coord1 + scale * textindent;
				}
				else if (offsetx + coord2 > width - scale * textindent)
				{
					offsetx = -(coord2 - (width - scale * textindent));
				}
			}
			if (offsetx + tw < width - scale * textindent * 2)
			{
				offsetx = (width - tw - scale * textindent * 2);
			}
		}
		else
		{
			offsetx = 0;
		}
	}

	void TextField::GainFocus() {}

	void TextField::LoseFocus()
	{
		if ((style & TEXTFIELD_TEXTCHANGEACTIONEVENT) == 0 and (style & TEXTFIELD_READONLY) == 0 and (style & TEXTFIELD_LOSEFOCUSACTIONEVENT) != 0)
		{
			EmitEvent(EVENT_WIDGETACTION, Self(), 0, 0, 0, 0, 0, make_shared<WString>(text));
		}
	}

	void TextField::MouseEnter(const int x, const int y)
	{
		Widget::MouseEnter(x, y);
		//if ((style & TEXTFIELD_READONLY) == 0)
		//{
		//	gui->SetPointer(POINTER_IBEAM);
		//}
	}

	void TextField::MouseLeave(const int x, const int y)
	{
		Widget::MouseLeave(x, y);
		//if ((style & TEXTFIELD_READONLY) == 0)
		//{
		//	gui->SetPointer(POINTER_DEFAULT);
		//}
	}

	void TextField::KeyUp(const KeyCode key)
	{
		if (key == KEY_CONTROL) ctrlpressed = false;
		if (key == KEY_SHIFT) shiftpressed = false;
	}

	void TextField::KeyDown(const KeyCode key)
	{
		if (key == KEY_CONTROL)
		{
			ctrlpressed = true;
		}
		if (key == KEY_A and ctrlpressed == true)
		{
			caretposition = text.size();
			sellen = -caretposition;
			Redraw();
		}
		if (key == KEY_SHIFT) shiftpressed = true;
		if (key == KEY_LEFT or key == KEY_UP)
		{
			if (shiftpressed == false and sellen != 0)
			{
				//Move the caret to the left side of the selection
				caretposition = Min(caretposition, caretposition + sellen);
				sellen = 0;
				//self.cursorblinkmode = true
				UpdateOffset();
				cursorblinkstate = true;
				gui->ResetCursorBlink();
				Redraw();
			}
			else
			{
				//Move the caret one character left
				if (caretposition > 0)
				{
					caretposition--;
					//self.cursorblinkmode = true
					if (shiftpressed)
					{
						sellen++;
					}
					else
					{
						sellen = 0;
					}
					UpdateOffset();
					cursorblinkstate = true;
					gui->ResetCursorBlink();
					Redraw();
				}
			}
		}
		else if (key == KEY_RIGHT or key == KEY_DOWN)
		{
			if (shiftpressed == false and sellen != 0)
			{
				//Move the caret to the right side of the selection
				caretposition = Max(caretposition, caretposition + sellen);
				sellen = 0;
				//self:GetGUI() : ResetCursorBlink()
				//self.cursorblinkmode = true
				UpdateOffset();
				cursorblinkstate = true;
				gui->ResetCursorBlink();
				Redraw();
			}
			else
			{
				//Move the caret one character right
				if (caretposition < text.length() )
				{
					caretposition++;
					//self:GetGUI() : ResetCursorBlink()
					//cursorblinkmode = true
					if (shiftpressed)
					{
						sellen--;
					}
					else
					{
						sellen = 0;
					}
					UpdateOffset();
					cursorblinkstate = true;
					gui->ResetCursorBlink();
					Redraw();
				}

			}
		}
		else if (key == KEY_ENTER)
		{
			sellen = 0;
			offsetx = 0;
			if ((style & TEXTFIELD_ENTERKEYACTIONEVENT) != 0)
			{
				if ((style & TEXTFIELD_LOSEFOCUSACTIONEVENT) == 0)
				{
					EmitEvent(EVENT_WIDGETACTION, Self(), 0, 0, 0, 0, 0, make_shared<WString>(text));
				}
			}
			gui->SetFocus(NULL);
			//EmitEvent(EVENT_WIDGET_ACTION,Self(),0,0,0,0,0,make_shared<WString>(text));
		}
		Widget::KeyDown(key);
	}

	void TextField::Paste()
	{
		if ((TEXTFIELD_READONLY & style) != 0) return;
		if (sellen != 0) KeyChar('\b');
		auto text = GetClipboardText();
		text = text.Replace("\n", "");
		text = text.Replace("\r", "");
		if (text == "") return;
		WString left = this->text.Left(caretposition);
		WString right = this->text.Right(this->text.size() - caretposition);
		caretposition += text.size();
		SetText(left + text + right);
	}

	void TextField::Copy()
	{
		if ((TEXTFIELD_PASSWORD & style) != 0) return;
		auto text = GetText(true);
		if (text == "") return;
		SetClipboardText(text);
	}

	void TextField::Cut()
	{
		if ((TEXTFIELD_PASSWORD & style) != 0) return;
		if (sellen == 0) return;
		if ((TEXTFIELD_READONLY & style) != 0) return;
		Copy();
		KeyChar('\b');
	}

	void TextField::KeyChar(const int charcode)
	{
		if ((TEXTFIELD_READONLY & style) != 0) return;

		auto s = text;
		auto c = WChr(charcode);
		if (c == "\x1a") return;// undo
		if (c == "\x16") return;// paste
		if (c == "\b")
		{
			//Backspace
			if (s.length() > 0)
			{
				if (sellen == 0)
				{
					if (caretposition == s.length())
					{
						s = s.Left( s.length() - 1);
					}
					else if (caretposition > 0)
					{						
						s = s.Left( caretposition - 1) + s.Right( s.length() - caretposition);
					}
					caretposition = caretposition - 1;
					caretposition = Max(0, caretposition);
				}
				else
				{
					auto c1 = Min(caretposition, caretposition + sellen);
					auto c2 = Max(caretposition, caretposition + sellen);
					s = s.Left( c1) + s.Right( s.length() - c2);
					caretposition = c1;
					sellen = 0;
				}
				//cursorblinkmode = true;
				if (text != s)
				{
					m_text = s;
					cursorblinkstate = true;
					gui->ResetCursorBlink();
					UpdateOffset();
					Redraw();
					if ((style & TEXTFIELD_TEXTCHANGEACTIONEVENT) != 0) EmitEvent(EVENT_WIDGETACTION, Self(), 0, 0, 0, 0, 0, make_shared<WString>(text));
				}
			}
		}
		else if (c != "\r" and c != "")
		{
			/*if ((style & TEXTFIELD_NUMBER) != 0)
			{
				if (charcode < 48 and charcode > 57 and charcode != 46) return;
			}
			if ((style & TEXTFIELD_INTEGER) != 0)
			{
				if (charcode < 48 and charcode > 57) return;
			}*/

			//What was I thinking here???
			/*switch (charcode)
			{
			case '<':
			case '>':
			case ':':
			case '"':
			case '/':
			case '\\':
			case '|':
			case '?':
				return;
			}*/
			
			//Insert a new character
			auto c1 = Min(caretposition, caretposition + sellen);
			auto c2 = Max(caretposition, caretposition + sellen);
			s = s.Left(c1) + c + s.Right(s.length() - c2);
			caretposition = caretposition + 1;
			if (sellen < 0) caretposition = caretposition + sellen;
			sellen = 0;
			//--self:GetGUI() : ResetCursorBlink();
			//cursorblinkmode = true;
			cursorblinkstate = true;
			gui->ResetCursorBlink();
			if (text != s)
			{
				m_text = s;
				UpdateOffset();
				Redraw();
				if ((style & TEXTFIELD_TEXTCHANGEACTIONEVENT) != 0) EmitEvent(EVENT_WIDGETACTION, Self(), 0, 0, 0, 0, 0, make_shared<WString>(text));
			}
		}
	}

	WString TextField::GetText(const bool selected)
	{
		if (selected)
		{
			if (sellen > 0)
			{
				return text.Mid(caretposition, sellen);
			}
			else if (sellen < 0)
			{
				return text.Mid(caretposition + sellen, -sellen);
			}
			else
			{
				return "";
			}
		}
		else
		{
			return Widget::GetText(selected);
		}
	}

	void TextField::SetText(const WString& text)
	{
		Widget::SetText(text.Replace("\n", "").Replace("\r", ""));
	}

	void TextField::SelectText(const int position, const int length)
	{
		caretposition = Max(position,0);
		if (length == -1)
		{
			sellen = text.size() - caretposition;
		}
		else
		{
			sellen = Min(length, text.size()) - caretposition;
		}
		sellen = Max(0, sellen);
		caretposition += sellen;
		sellen *= -1;
	}

	WString TextField::GetSelectedText()
	{
		if (sellen == 0) return "";
		if (Abs(sellen) == text.size()) return text;
		int c1 = Min(caretposition, caretposition + sellen);
		int c2 = Max(caretposition, caretposition + sellen);
		return text.Mid(c1, c2 - c1);
	}

	void TextField::Draw(const int x, const int y, const int width, const int height)
	{
		//Background
		blocks[0].color = color[WIDGETCOLOR_SUNKEN];
		blocks[0].position = iVec2(0);
		blocks[0].size = size;
		blocks[0].radius = cornerradius;
		blocks[0].hidden = false;
		if (pushstate)
		{
			blocks[0].color = color[WIDGETCOLOR_BACKGROUND] * 0.75f;
		}

		//Selection
		blocks[1].hidden = true;
		if (sellen != 0)
		{
			blocks[1].color = color[WIDGETCOLOR_HIGHLIGHT];
			auto fragment = GetSelectedText();
			int c1 = Min(caretposition, caretposition + sellen);
			int c2 = Max(caretposition, caretposition + sellen);
			auto prefix = text.Left(c1);
			blocks[1].hidden = false;
			blocks[1].position.x = Round(float(textindent) * gui->scale) + gui->GetTextWidth(font, fontscale, prefix, fontweight) + offsetx;
			blocks[1].position.y = (size.y - gui->GetFontHeight(font, fontscale, fontweight)) / 2;
			blocks[1].size.x = gui->GetTextWidth(font, fontscale, fragment, fontweight);
			blocks[1].size.y = gui->GetFontHeight(font, fontscale, fontweight);
		}

		//Caret
		if ((style & TEXTFIELD_READONLY) == 0)
		{
			if (/*sellen == 0 and */gui->GetFocus() == Self() and cursorblinkstate == true)
			{
				blocks[2].hidden = false;
				blocks[2].position.x = GetCaretCoord() + offsetx;
				blocks[2].position.y = (size.y - gui->GetFontHeight(font, fontscale, fontweight)) / 2 - 1;
				blocks[2].size.x = 1;
				blocks[2].size.y = gui->GetFontHeight(font, fontscale, fontweight) + 2;
				blocks[2].color = color[WIDGETCOLOR_FOREGROUND];
			}
			else
			{
				blocks[2].hidden = true;
			}
		}
		else
		{
			blocks[2].hidden = true;
		}

		//Label
		if (text.empty())
		{
			blocks[3].hidden = true;
		}
		else
		{
			blocks[3].color = color[WIDGETCOLOR_FOREGROUND];
			blocks[3].position = iVec2(0) + cornerradius;
			blocks[3].position.x += Round(float(textindent) * gui->scale) + offsetx;
			blocks[3].textdefined = true;
			blocks[3].size = size - cornerradius * textindent;
			blocks[3].size.x -= 4;
			blocks[3].size.x -= offsetx;
			if ((style & TEXTFIELD_PASSWORD) != 0)
			{
				blocks[3].text = std::wstring(text.size(), L'•');
			}
			else
			{
				blocks[3].text = text;
			}
			blocks[3].textalignment = TEXT_LEFT | TEXT_MIDDLE;
			blocks[3].hidden = false;
		}

		//Border
		if (gui->GetFocus() == Self() and ((style & TEXTFIELD_READONLY) == 0))
		{
			blocks[4].color = color[WIDGETCOLOR_SELECTION];
		}
		else
		{
			blocks[4].color = color[WIDGETCOLOR_BORDER];
		}
		blocks[4].position = iVec2(0);
		blocks[4].size = size;
		blocks[4].wireframe = true;
		blocks[4].radius = cornerradius;
		blocks[4].hidden = false;
	}
}