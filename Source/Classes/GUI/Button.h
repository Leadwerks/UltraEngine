#pragma once
#include "UltraEngine.h"

namespace UltraEngine
{
	class Widget;
	class Window;
	class Event;
	class Mouse;
	class iVec2;

	enum ButtonStyle
	{
		BUTTON_DEFAULT = 0,
		BUTTON_TOGGLE = 1,
		BUTTON_RADIO = 2,
		BUTTON_CHECKBOX = 4,
		BUTTON_OK = 8,
		BUTTON_CANCEL = 16,
		BUTTON_TOOLBAR = 32
	};
	inline ButtonStyle operator|(ButtonStyle a, ButtonStyle b)
	{
		return static_cast<ButtonStyle>(static_cast<int>(a) | static_cast<int>(b));
	};

	class Button : public Widget
	{
		bool hoverstate, pushstate;
		int checkboxsize;

	protected:
		virtual void Reset();
	public:
		Button();

		virtual void MouseMove(const int x, const int y);
		virtual void MouseEnter(const int x, const int y);
		virtual void MouseLeave(const int x, const int y);
		virtual void MouseDown(const MouseButton button, const int x, const int y);
		virtual void MouseUp(const MouseButton button, const int x, const int y);
		virtual void LoseFocus();
		virtual void Draw(const int x, const int y, const int width, const int height);
		virtual void KeyDown(const KeyCode key);
		virtual void SetState(const WidgetState state);
		
		friend shared_ptr<Widget> CreateButton(const WString&, const int, const int, const int, const int, shared_ptr<Widget>, const ButtonStyle);
		friend shared_ptr<Widget> CreateButton(const WString&, const int, const int, const int, const int, shared_ptr<Interface>, const ButtonStyle);
	};

	extern shared_ptr<Widget> CreateButton(const WString& text, const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const ButtonStyle style = BUTTON_DEFAULT);
	extern shared_ptr<Widget> CreateButton(const WString& text, const int x, const int y, const int width, const int height, shared_ptr<Interface> ui, const ButtonStyle style = BUTTON_DEFAULT);
}
#ifdef LUA_VERSION
SOL_BASE_CLASSES(UltraEngine::Button, UltraEngine::Widget, UltraEngine::Object);
#endif
