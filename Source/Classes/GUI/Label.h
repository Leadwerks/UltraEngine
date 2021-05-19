#pragma once
#include "UltraEngine.h"

namespace UltraEngine
{
	class Widget;
	class Window;
	class Event;
	class Mouse;
	class iVec2;
	
	enum LabelStyle
	{
		LABEL_LEFT = TEXT_LEFT,
		LABEL_DEFAULT = LABEL_LEFT,
		LABEL_RIGHT = TEXT_RIGHT,
		LABEL_CENTER = TEXT_CENTER,
		LABEL_TOP = TEXT_TOP,
		LABEL_BOTTOM = TEXT_BOTTOM,
		LABEL_MIDDLE = TEXT_MIDDLE,
		LABEL_BORDER = 128
	};
	inline LabelStyle operator|(LabelStyle a, LabelStyle b)
	{
		return static_cast<LabelStyle>(static_cast<int>(a) | static_cast<int>(b));
	};

	class Label : public Widget
	{
	public:
		Label();

		virtual void Draw(const int x, const int y, const int width, const int height);
	};

	extern shared_ptr<Widget> CreateLabel(const WString& text, const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const LabelStyle style = LABEL_DEFAULT);
	//extern shared_ptr<Widget> CreateLabel(const WString& text, const int x, const int y, const int width, const int height, shared_ptr<Interface> ui, const LabelStyle style = LABEL_DEFAULT);
}
#ifdef LUA_VERSION
SOL_BASE_CLASSES(UltraEngine::Label, UltraEngine::Widget, UltraEngine::Object);
#endif
