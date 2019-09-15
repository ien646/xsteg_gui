#pragma once

#include <guim/widget.hpp>
#include <guim/traits/background_color.hpp>
#include <guim/traits/foreground_color.hpp>
#include <functional>
#include <string>
#include <vector>

namespace guim
{
    class selectable : public widget
    {
    protected:
        std::string _label;
        std::vector<std::function<void()>> _callbacks;

    public:
		traits::background_color background_color;
		traits::foreground_color foreground_color;

        selectable(const std::string& label);
        virtual void update() override;

        void operator+=(std::function<void()> cback);
        void add_callback(std::function<void()> cback);
    };
}