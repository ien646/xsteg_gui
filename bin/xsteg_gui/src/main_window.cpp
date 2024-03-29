#include "main_window.hpp"

#include <guim/button.hpp>
#include "encode_window.hpp"

main_window::main_window(guim::appwindow* appwnd)
    : _appwnd(appwnd)
    , window("main_window", ImVec2(0, 0))
{ }

void main_window::update()
{
    static bool first_time = true;
    static guim::button* button_encode = add_widget<guim::button>(std::string("Encode"));
    static guim::button* button_decode = add_widget<guim::button>(std::string("Decode"));
    static guim::button* button_genmaps = add_widget<guim::button>(std::string("Generate maps"));
    static encode_window* encode_win = nullptr;

    if(!enabled) { return; }
    
    if(first_time)
    {
        if(_position)
        {
            glfwSetWindowPos(_appwnd->wnd_ptr(), (int)_position->x, (int)_position->y);
        }

        first_time = false;
        auto [x, y] = _appwnd->get_window_size();
        set_size(ImVec2((float)x, (float)y));
        set_position(ImVec2(0, 0));

        this->background = false;
        this->titlebar = false;
        this->resizable = false;
        this->never_on_front = true;

        button_encode->sameline = true;
        button_decode->sameline = true;
        button_genmaps->sameline = true;
        button_encode->set_size(ImVec2((float)(x / 3) - 2, 0));
        button_decode->set_size(ImVec2((float)(x / 3) - 2, 0));
        button_genmaps->set_size(ImVec2((float)(x / 3) - 8, 0));

        button_encode->add_callback([&]()
        {
            if(encode_win == nullptr)
            {
                auto [x, y] = _appwnd->get_window_size();
                encode_win = add_widget<encode_window>(ImVec2((float)x / 1.25F, (float)y / 1.25F));
            }
            else
            {
                encode_win->enabled = true;
            }
        });        
    }
	window::update();
}