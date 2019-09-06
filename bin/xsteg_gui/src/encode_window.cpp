#include "encode_window.hpp"

#include <vector>
#include <xsteg/availability_map.hpp>

#include "threshold_editor.hpp"
#include "application_window.hpp"
#include "browser_window.hpp"

encode_window::encode_window(application_window* wnd, const std::string& name)
    : imgui_window(wnd, name)
{ }

encode_window::encode_window(application_window* wnd, std::string&& name)
    : imgui_window(wnd, std::move(name))
{ }

void encode_window::show()
{
    imgui_window::show();
    _first_time = true;
}

void encode_window::update_proc()
{
    static bool btn_browse_input = false;
    static bool btn_browse_output = false;
    static bool btn_browse_data = false;
    static bool btn_edit_thresh = false;
    static bool btn_gen_key = false;
    static bool btn_res_key = false;
    static bool btn_encode = false;
    static threshold_editor th_editor;
    
    /*if(_first_time)
    {
        auto [x, y] = _wnd->get_window_size();
        float win_x = static_cast<float>(x * 0.50);
        float win_y = static_cast<float>(y * 0.60);
        ImGui::SetWindowSize(ImVec2(win_x, win_y));
    }*/
    
    // **** Input image path ****
    ImGui::Text("Input image:   ");
    ImGui::SameLine();
    ImGui::InputTextWithHint("##ii", "Input image path", _input_image.data(), _input_image.size() + 1);
    ImGui::SameLine();
    btn_browse_input = ImGui::Button("Browse...##-ii");

    // **** Output image path ****
    ImGui::Text("Output image:  ");
    ImGui::SameLine();
    ImGui::InputTextWithHint("##oi", "Output image path", _output_image.data(), _output_image.size() + 1);
    ImGui::SameLine();
    btn_browse_output = ImGui::Button("Browse...##-oi");

    // **** Input data file ****
    ImGui::Text("Data file:     ");
    ImGui::SameLine();
    ImGui::InputTextWithHint("##df", "Input data file", _input_data.data(), _input_data.size() + 1);
    ImGui::SameLine();
    btn_browse_data = ImGui::Button("Browse...##-df");

    ImGui::Separator();
    ImGui::Spacing();

    // **** Threshold editor ****
    ImGui::Text("Thresholds: (%d)", th_editor.threshold_count());

    th_editor.update();

    auto bits = th_editor.truncated_bits();
    ImGui::Text("Truncated bits: R[%d] G[%d] B[%d] A[%d]", bits.r, bits.g, bits.b, bits.a);
    ImGui::Separator();

    if(ImGui::Button("Generate key"))
    {
        ImGui::OpenPopup("gen_key_popup");
    }
    ImGui::SameLine();

    if(ImGui::Button("Restore key"))
    {
        ImGui::OpenPopup("res_key_popup");
    }
    ImGui::Separator();

    btn_encode = ImGui::Button("Encode!");

    if(btn_browse_input)
    {
        browser_window* fwin = add_child_window<browser_window>(
            "_fopen_window_ii_", 
            _appwnd, 
            "Input file select", 
            &_input_image,
            browser_window_mode::FILE_SELECT
        );
        fwin->show();
    }

    if(btn_browse_output)
    {
        browser_window* fwin = add_child_window<browser_window>(
            "_fopen_window_oi_", 
            _appwnd,
            "Output image select",
            &_output_image,
            browser_window_mode::FILE_SAVE
        );
        fwin->show();
    }

    if(btn_browse_data)
    {
        browser_window* fwin = add_child_window<browser_window>(
            "_fopen_window_df_", 
            _appwnd,
            "Data file select",
            &_input_data,
            browser_window_mode::FILE_SELECT
        );
        fwin->show();
    }

    if(ImGui::BeginPopupModal("gen_key_popup", NULL, ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize))
    {
        std::string key = xsteg::generate_thresholds_key(th_editor.thresholds());
        ImGui::Text("Generated key:");
        ImGui::InputText(
            "##gen_key", 
            key.data(), 
            key.size(), 
            ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
        if (ImGui::Button("Close"))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    if(ImGui::BeginPopupModal("res_key_popup", NULL, ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize))
    {
        static std::string key(1024, 0);
        ImGui::Text("Key: ");
        ImGui::SameLine();
        ImGui::InputText("##res_key", key.data(), key.size());
        if (ImGui::Button("Restore"))
        {
            th_editor.thresholds(xsteg::parse_thresholds_key(std::string(key.c_str())));
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if(ImGui::Button("Close"))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}