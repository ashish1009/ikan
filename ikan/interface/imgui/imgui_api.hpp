//
//  Imgui_api.hpp
//  ikan
//
//  Created by Ashish . on 05/04/23.
//

#pragma once

namespace ikan {
    
  /// Imgui Wrapper class
  class ImguiAPI {
  public:
    /// This function Enables the docking in Imgui. To be called each frame before every imgui rendering;
    static void StartDcocking();
    /// This function Disable the docking in Imgui. To be called each frame after every imgui rendering;
    static void EndDcocking();

    /// This function renders a widgit that shows the frame rate of the application
    /// - Parameter is_open: flag to show or hide the widget
    static void Framerate(bool *is_open);

    /// This function changes the imgui font
    ///  - Parameters:
    ///   - default_font_path: Default font file information
    ///   - bold_font_path: bold font file information
    static void ChangeFont(const Font& default_font_path, const Font& bold_font_path);

    /// This fucntions sets the Grey theme of Imgui
    static void SetGreyThemeColors();
    /// This fucntions sets the Light Grey theme of Imgui
    static void SetLightGreyThemeColors();
    /// This fucntions sets the Light theme of Imgui
    static void SetLightThemeColors();
    /// This fucntions sets the Dark theme of Imgui
    static void SetDarkThemeColors();

    template<typename UIFunction>
    /// This function wraps the Imgui Begin Menu
    /// - Parameters:
    ///   - menu_title: Menu title
    ///   - enable: Is menu enabled
    ///   - func: function to called when menu clicked
    static void Menu(const std::string& menu_title, bool enable, UIFunction func) {
      if (ImGui::BeginMenu(menu_title.c_str(), enable)) {
        func();
        ImGui::EndMenu();
      }
    }
    
    MAKE_PURE_STATIC(ImguiAPI);
  };
  
}
