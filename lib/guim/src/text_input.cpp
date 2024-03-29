#include <guim/text_input.hpp>

#include <imgui.h>
#include <imgui_stdlib.h>

namespace guim
{
	std::string& text_input::text()
	{
		return _text;
	}

	void text_input::update()
	{
		if(!enabled) { return; }
		static std::string prev_text = _text;
		if(prev_text != _text && _edit_callback != nullptr)
		{
			_edit_callback(_text);
		}
		prev_text = _text;
		
		background_color.push();
        foreground_color.push();

		ImGuiInputTextFlags flags = 0;
		if(readonly)
		{
			flags |= ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly;
		}
		if(_size.x > 0)
		{
			ImGui::SetNextItemWidth(_size.x);
		}
		ImGui::InputText(_label.c_str(), _text.data(), _text.length() + 1, flags);

		background_color.pop();
        foreground_color.pop();
			
		if(sameline)
		{
			ImGui::SameLine();
		}
	}
}