#pragma once
#include "UltraEngine.h"

namespace UltraEngine
{
	class Widget;
	class Window;
	class Event;
	class Mouse;
	class iVec2;

	enum TextFieldStyle
	{
		TEXTFIELD_READONLY = 1,
		//TEXTFIELD_PASSWORD = 2,
		TEXTFIELD_TEXTCHANGEACTIONEVENT = 4,
		TEXTFIELD_LOSEFOCUSACTIONEVENT = 8,
		TEXTFIELD_ENTERKEYACTIONEVENT = 16,
		TEXTFIELD_DEFAULT = TEXTFIELD_LOSEFOCUSACTIONEVENT + TEXTFIELD_ENTERKEYACTIONEVENT,
		//TEXTFIELD_NUMBER = 8,
		//TEXTFIELD_INTEGER = 16,
		//TEXTFIELD_FILEPATH = 32,
		TEXTFIELD_PASSWORD = 64
	};
	inline TextFieldStyle operator|(TextFieldStyle a, TextFieldStyle b)
	{
		return static_cast<TextFieldStyle>(static_cast<int>(a) | static_cast<int>(b));
	};

	class TextField : public Widget
	{
		bool ctrlpressed;
		bool pressed;
		iVec2 lastmouseposition;
		int textindent, caretposition, sellen, x, offsetx;
		bool hoverstate, pushstate, shiftpressed, cursorblinkstate;
		int doubleclickrange;
		WString GetSelectedText();
		int GetCharAtPosition(const int pos, const bool clickonchar = false);
		int GetCaretCoord();
		int GetCaretCoord(const int caret);
		void UpdateOffset();
	protected:
		virtual bool OK();
	public:
		TextField();

		virtual WString GetText(const bool selected = false);
		virtual void SelectText(const int position, const int length);
		virtual void KeyUp(const KeyCode key);
		virtual void MouseDown(const MouseButton button, const int x, const int y);
		virtual void DoubleClick(const MouseButton button, const int x, const int y);
		virtual void TripleClick(const MouseButton button, const int x, const int y);
		virtual void MouseUp(const MouseButton button, const int x, const int y);
		virtual void MouseMove(const int x, const int y);
		virtual void GainFocus();
		virtual void LoseFocus();
		virtual void MouseEnter(const int x, const int y);
		virtual void MouseLeave(const int x, const int y);
		virtual void KeyDown(const KeyCode key);
		virtual void KeyChar(const int keychar);
		virtual void CursorBlink();
		virtual void Cut(), Copy(), Paste();
		virtual void SetText(const WString& text);

		virtual void Draw(const int x, const int y, const int width, const int height);
	};

	extern shared_ptr<Widget> CreateTextField(const int x, const int y, const int width, const int height, shared_ptr<Interface> ui, const TextFieldStyle style = TEXTFIELD_DEFAULT);
	extern shared_ptr<Widget> CreateTextField(const int x, const int y, const int width, const int height, shared_ptr<Widget> ui, const TextFieldStyle style = TEXTFIELD_DEFAULT);
}
#ifdef LUA_VERSION
SOL_BASE_CLASSES(UltraEngine::TextField, UltraEngine::Widget, UltraEngine::Object);
#endif