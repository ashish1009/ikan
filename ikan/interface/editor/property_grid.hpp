//
//  property_grid.hpp
//  ikan
//
//  Created by Ashish . on 05/04/23.
//

#pragma once

#include <imgui_internal.h>

namespace ikan {
  
  /// This class stores the Wrapper API for showing Data in Imgui window
  class PropertyGrid {
  public:
    /// This function render a hint message when hovered last imgui item
    /// - Parameters:
    ///   - description: String to be used as hint
    ///   - error: flag to render text in red
    static void HoveredMsg(const char* description, bool error = false);
    /// This function render a hint icon (?) and on hovered A hint string will be shown in subwindow
    /// on hovering the mouse
    /// - Parameters:
    ///   - description: String to be used as hint
    ///   - icon_char: Icon to be printed as Help Marker (By default (?) will be rendered)
    ///   - same_line: check for same line icon or in new line
    static void HelpMarker(const char* description, const char* icon_char = "?", bool same_line = true);

    /// This fucntion renders the CheckBox to select item
    /// - Parameters:
    ///   - label: Lable for Variable
    ///   - value: Value reference to be changed
    ///   - column_width_2: width of column of lable
    static bool CheckBox(const char* label, bool& value,
                         float column_width_1 = ImGui::GetWindowContentRegionMax().x / 2);

    /// This function renderes a image button
    /// - Parameters:
    ///   - lableId: image lable string
    ///   - texId: texture id
    ///   - size: size of image
    ///   - bgColor: background color
    ///   - padding: padding
    static bool ImageButton(const std::string& lable_id, uint32_t texId, const glm::vec2& size,
                            const glm::vec4& bgColor = glm::vec4(0.0f), int32_t padding = 0,
                            const glm::vec2& uv0 = glm::vec2(0.0f, 1.0f), const glm::vec2& uv1 = glm::vec2(1.0f, 0.0f));
    /// This funcrtion renders a Image Button with lable ID
    /// - Parameters:
    ///   - lableId: lable ID
    ///   - texId: Texture ID
    ///   - size: size of textire
    static bool ImageButton(const int32_t lable_id, uint32_t texId, const glm::vec2& size,
                            const glm::vec2& uv0 = glm::vec2(0.0f, 1.0f), const glm::vec2& uv1 = glm::vec2(1.0f, 0.0f));

    /// This function renders slider for float
    /// - Parameters:
    ///   - label: Lable for Variable
    ///   - value: Value reference to be changed
    ///   - checkbox_flag: flag to render either checkbox or lable
    ///   - delta: step of Drag in float
    ///   - reset_value: value of reset button pressed
    ///   - min_value: min value
    ///   - max_value: max value
    ///   - column_width: width of column of lable
    static bool Float1(const char* label, float& value, bool* checkbox_flag = nullptr, float delta = 0.1f, float reset_value = 0.0f,
                       float min_value = MIN_FLT, float max_value = MAX_FLT, float column_width = ImGui::GetWindowContentRegionMax().x / 2);
    /// This function renders slider for float 3
    /// - Parameters:
    ///   - label: Lable for Variable
    ///   - value: Value reference to be changed
    ///   - checkbox_flag: flag to render either checkbox or lable
    ///   - delta: step of Drag in float
    ///   - reset_value: value of reset button pressed
    ///   - min_value: min value
    ///   - max_value: max value
    ///   - column_width: width of column of lable
    static bool Float2(const char* label, glm::vec2& value, bool* checkbox_flag = nullptr, float delta = 0.1f, float reset_value = 0.0f,
                       float min_value = MIN_FLT, float max_value = MAX_FLT, float column_width = ImGui::GetWindowContentRegionMax().x / 2);
    /// This function renders slider for float 3
    /// - Parameters:
    ///   - label: Lable for Variable
    ///   - value: Value reference to be changed
    ///   - checkbox_flag: flag to render either checkbox or lable
    ///   - delta: step of Drag in float
    ///   - reset_value: value of reset button pressed
    ///   - min_value: min value
    ///   - max_value: max value
    ///   - column_width: width of column of lable
    static bool Float3(const char* label, glm::vec3& value, bool* checkbox_flag = nullptr, float delta = 0.1f, float reset_value = 0.0f,
                       float min_value = MIN_FLT, float max_value = MAX_FLT, float column_width = ImGui::GetWindowContentRegionMax().x / 2);
    
    /// This function renders slider for float
    /// - Parameters:
    ///   - label: Lable for Variable
    ///   - value: Value reference to be changed
    ///   - reset_value: value to reset
    ///   - min_value: min value
    ///   - max_value: max value
    ///   - column_width: width of column of lable
    static bool SliderFloat1(const char* label, float& value, float reset_value = 0.0f, float min_value = MIN_FLT,
                             float max_value = MAX_FLT, float column_width = ImGui::GetWindowContentRegionMax().x / 2);
    /// This function renders slider for float 3
    /// - Parameters:
    ///   - label: Lable for Variable
    ///   - value: Value reference to be changed
    ///   - checkbox_flag: flag to render either checkbox or lable
    ///   - reset_value: value to reset
    ///   - min_value: min value
    ///   - max_value: max value
    ///   - column_width: width of column of lable
    static bool SliderFloat2(const char* label, glm::vec2& value, float reset_value = 0.0f, float min_value = MIN_FLT,
                             float max_value = MAX_FLT, float column_width = ImGui::GetWindowContentRegionMax().x / 2);
    /// This function renders slider for float 3
    /// - Parameters:
    ///   - label: Lable for Variable
    ///   - value: Value reference to be changed
    ///   - reset_value: value to reset
    ///   - min_value: min value
    ///   - max_value: max value
    ///   - column_width: width of column of lable
    static bool SliderFloat3(const char* label, glm::vec3& value, float reset_value = 0.0f, float min_value = MIN_FLT,
                             float max_value = MAX_FLT, float column_width = ImGui::GetWindowContentRegionMax().x / 2);

    /// This function renderes a search box with search button
    /// - Parameters:
    ///   - value: value written in the box
    ///   - hint: hint to be shown
    ///   - size: size of search button
    static bool Search(char* value, const char* hint, const glm::vec2& size = { 15.0f, 15.0f });
    /// This function renders Imgae in current Imgui window
    /// - Parameters:
    ///   - textureID; Texture id of Image to be rendered
    ///   - size; size of image need to be rendered inside Imgui window (this size will be visible as texture)
    ///   - uv0; Left
    ///   - uv1; Right
    static void Image(void* textureID, const glm::vec2& size, const glm::vec2& uv0 = glm::vec2( 0, 1 ),
                      const glm::vec2& uv1 = glm::vec2( 1, 0 ));

    /// This function renders the text box
    /// - Parameters:
    ///   - value: Value need to be edit or entered in box
    ///   - label: Lable for Variable
    ///   - num_columns: we can add extra column if needed. If this is more then 2 then reset the column to back to 1 after calling
    ///   - column_width1: width of column of lable
    ///   - hint: Hint to be shown in the text box. Null of no hind is given
    ///   - modifiable: flag to check is string is constant or modifiable
    ///   - multiple: flag to check if there are multiple line in text box
    ///   - num_lines: if multipleLine is true then check number of line are there in text box
    ///   - error: flag to check if text is error (if true text printed as red)
    /// - Important: If num columns are more than 2 then kindly use those columns outside and set column to 1 again,
    ///              this API will use only 2 columns and make column 1 only in case of num columns are 2
    static bool TextBox(std::string& value, const char* label = nullptr, uint32_t num_columns = 2, float column_width_1 = 100.0f,
                        const char* hint = nullptr, bool modifiable = true, bool multiple = false, int32_t num_lines = 1, bool error = false);

    /// This function renders the Text box to enter string
    /// - Parameters:
    ///   - label: Lable for Variable
    ///   - value: Value need to be edit or entered in box
    ///   - hint: Hint to be shown in the text box. Null of no hind is given
    ///   - columnWidth: width of column of lable
    static bool ReadOnlyTextBox(const char* label, const std::string& value, const char* hint = nullptr,
                                float column_width = ImGui::GetWindowContentRegionMax().x / 2);

    /// ComboDrop: Selectable drop option created
    /// - Parameters:
    ///   - label: is string to be printed
    ///   - options: option of drop box
    ///   - current_value: current selection value (index of option vector)
    ///   - column_width: column width
    static uint32_t ComboDrop(const char* label, const std::vector<std::string>& options, uint32_t current_value,
                              float column_width = ImGui::GetWindowContentRegionMax().x / 2);
    
    /// This function catch the dragged content from content prowser pannel and call the function passed as ui_function
    /// - Parameter uiFunction: Function
    template<typename UIFunction> static void DropConent(UIFunction ui_function) {
      if (ImGui::BeginDragDropTarget() and
          !ImGui::IsMouseDragging(0) and
          ImGui::IsMouseReleased(0)) {
        if (const ImGuiPayload* data = ImGui::AcceptDragDropPayload("SelectedFile", ImGuiDragDropFlags_AcceptBeforeDelivery)) {
          char* file_path = new char[uint32_t(data->DataSize + 1)];
          memcpy(file_path, (char*)data->Data, (size_t)data->DataSize);
          file_path[data->DataSize] = '\0';
          
          ui_function(file_path);
          delete[] file_path;
        }
        ImGui::EndDragDropTarget();
      }
    }

  private:
    static bool FloatImpl(const std::vector<std::string>& buttons, const char* label, const std::vector<float*>& values,
                          bool* checkbox_flag, float delta, float reset_value, float min_value, float max_value, float column_width);
    static bool SliderFloatImpl(const std::vector<std::string>& buttons, const char* label, const std::vector<float*>& values,
                                float reset_value , float min_value, float max_value, float column_width);
  };
  
} // namespace ikan
