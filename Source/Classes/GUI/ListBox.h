#pragma once
#include "UltraEngine.h"

namespace UltraEngine
{
	class Widget;
	class Window;
	class Event;
	class Mouse;
	class iVec2;

	class Slider;
	class ComboBox;

	class ListBox : public Widget
	{
		bool rebuild;
		int textindent;
		bool hoverstate, pushstate;
		weak_ptr<ComboBox> combobox;
		shared_ptr<Slider> slider;
		int sliderwidth;
		int offset;
		int defaultitemheight;

		virtual void UpdateSlider();
		virtual int ItemHeight();
	protected:
		virtual void DoubleClick(const MouseButton button, const int x, const int y);
		virtual void Reset();
	public:
		ListBox();

		virtual void FilterItems(const WString& filter);
		virtual void ModifyItem(const int index, const WString& text);
		virtual void ClearItems();
		virtual bool ProcessEvent(const Event& e);
		virtual void MouseWheel(const int delta, const int x, const int y);
		virtual void UpdateLayout();
		virtual void SelectItem(const int index);
		virtual int AddItem(const WString& text, const bool selected = false);
		virtual void GainFocus();
		virtual void KeyDown(const KeyCode key);
		virtual void MouseEnter(const int x, const int y);
		virtual void MouseLeave(const int x, const int y);
		virtual void MouseMove(const int x, const int y);
		virtual void MouseDown(const MouseButton button, const int x, const int y);
		virtual void MouseUp(const MouseButton button, const int x, const int y);
		virtual void LoseFocus();
		virtual void Draw(const int x, const int y, const int width, const int height);
		virtual void SetShape(int x, int y, int width, int height);
		virtual bool OK();

		friend ComboBox;
		friend shared_ptr<Widget> CreateListBox(const int, const int, const int, const int, shared_ptr<Widget>, const ListBoxStyle);
		//friend shared_ptr<Widget> CreateListBox(const int, const int, const int, const int, shared_ptr<Interface>, const ListBoxStyle);
	};

	extern shared_ptr<Widget> CreateListBox(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const ListBoxStyle style = LISTBOX_DEFAULT);
	//extern shared_ptr<Widget> CreateListBox(const int x, const int y, const int width, const int height, shared_ptr<Interface> ui, const ListBoxStyle style = LISTBOX_DEFAULT);
}
#ifdef LUA_VERSION
SOL_BASE_CLASSES(UltraEngine::ListBox, UltraEngine::Widget, UltraEngine::Object);
#endif