#include <guim/separator.hpp>

namespace guim
{
    separator::separator()
        : widget()
    { }

    void separator::update()
    {
		if(!enabled) { return; }
        ImGui::Separator();
    }
}