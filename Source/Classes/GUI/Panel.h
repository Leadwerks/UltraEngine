#pragma once
#include "UltraEngine.h"

namespace UltraEngine
{
	class Widget;
	class Window;
	class Event;
	class Mouse;
	class iVec2;

	enum PanelStyle
	{
		PANEL_DEFAULT = 0,
		PANEL_BORDER = 1,
		PANEL_GROUP = 2
	};
	inline PanelStyle operator|(PanelStyle a, PanelStyle b)
	{
		return static_cast<PanelStyle>(static_cast<int>(a) | static_cast<int>(b));
	};

	class Panel : public Widget
	{
	public:
		Panel();

		//virtual void MouseWheel(const int delta, const int x, const int y);
		//virtual void MouseMove(const int x, const int y);
		//virtual void MouseEnter(const int x, const int y);
		//virtual void MouseLeave(const int x, const int y);
		//virtual void MouseDown(const MouseButton button, const int x, const int y);
		//virtual void MouseUp(const MouseButton button, const int x, const int y);
		//virtual void DoubleClick(const MouseButton button, const int x, const int y);
		//virtual void TripleClick(const MouseButton button, const int x, const int y);
		
		virtual void Draw(const int x, const int y, const int width, const int height);
	};

	extern shared_ptr<Panel> CreatePanel(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const PanelStyle style = PANEL_DEFAULT);
	//extern shared_ptr<Panel> CreatePanel(const int x, const int y, const int width, const int height, shared_ptr<Interface> ui, const PanelStyle style = PANEL_DEFAULT);
}
#ifdef LUA_VERSION
SOL_BASE_CLASSES(UltraEngine::Panel, UltraEngine::Widget, UltraEngine::Object);
#endif