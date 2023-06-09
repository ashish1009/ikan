//
//  content_browser_panel.cpp
//  ikan
//
//  Created by Ashish . on 07/04/23.
//

#include "content_browser_panel.hpp"
#include "editor/property_grid.hpp"
#include "renderer/graphics/texture.hpp"
#include "scene/scene_serialiser.hpp"
#include "scene/prefab.hpp"

namespace ikan {
  
#define CBP_DEBUG 0
  
  static constexpr float round_factor_ = 1.0f;
  static constexpr float window_y_offset_  = 55.0f;
  static constexpr glm::vec2 icon_size_ = {18.0f, 18.0f};
  
  static std::unordered_map<std::string, ImageData> prefab_image_map_;
  static std::unordered_map<std::string, std::shared_ptr<Texture>> texture_image_map_;

  /// This function removes the last slash from the directory if present
  /// - Parameter path: Directoruy path
  std::string RemoveLastSlash(const std::string& path) {
    std::string path_without_slash = path;
    if (path.at(path.length() - 1) == '/') {
      path_without_slash = path.substr(0, path.length() - 1);
    }
    return path_without_slash;
  }
  
  void ContentBrowserPanel::Shutdown() {
    prefab_image_map_.clear();
    texture_image_map_.clear();
  }
  
  void ContentBrowserPanel::SetRootPath(const std::string& root_path) {
    root_path_ = RemoveLastSlash(root_path);
    current_directory_ = RemoveLastSlash(root_path);
  }
  
  void ContentBrowserPanel::AddFavouritPaths(const std::vector<std::filesystem::path>& favourite_paths) {
    if (favourite_paths_.empty()) {
      favourite_paths_.emplace_back(RemoveLastSlash(DM::WorkspacePath("../ikan/")));
      favourite_paths_.emplace_back(RemoveLastSlash(DM::WorkspacePath("ikan/core_assets")));
    }
    for (const auto& path : favourite_paths)
      favourite_paths_.emplace_back(RemoveLastSlash(path));
  }

  void ContentBrowserPanel::AddAssetPaths(const std::vector<std::filesystem::path>& asset_paths) {
    for (const auto& path : asset_paths)
      asset_paths_.emplace_back(RemoveLastSlash(path));
  }
  
  void ContentBrowserPanel::ClearAllPaths() {
    asset_paths_.clear();
    favourite_paths_.clear();
  }

  void ContentBrowserPanel::RenderGui(bool* is_open) {
    CHECK_WIDGET_FLAG(is_open);
    
#if CBP_DEBUG
    ImGui::Begin("Content Broser Debug");
    
    ImGui::Text("Back paths");
    for (auto b : back_path_history_)
      ImGui::Text("%s", b.string().data());
    
    
    ImGui::Text("Forward paths");
    for (auto b : forward_path_history_)
      ImGui::Text("%s", b.string().data());
    
    ImGui::End();
#endif
    
    ImGui::Begin("Content Browser", is_open);
    ImGui::PushID("Content Browser");

    side_child_width_ = ImGui::GetContentRegionAvailWidth() * 0.20;

    TitleIcon();
    
    // Side Menue should be called before MainArea
    SideMenu();
    ImGui::SameLine();
    
    MainArea();

    ImGui::PopID(); // Content Browser
    ImGui::End(); // Content Browser
  }

  void ContentBrowserPanel::TitleIcon() {
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, round_factor_);
    ImGui::BeginChild("Title Area", ImVec2(ImGui::GetWindowContentRegionWidth(), 35.0f), true /* Border */, ImGuiWindowFlags_NoScrollbar);
    
    ImGui::Columns(3);
    ImGui::SetColumnWidth(0, 100);
    
    Back();             ImGui::SameLine();
    Forward();          ImGui::SameLine();
    Home();             ImGui::SameLine();
    
    ImGui::NextColumn();
    ImGui::SetColumnWidth(1, 200);
    
    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, (35.0f - 20.0f) / 2));
    Search();
    
    ImGui::NextColumn();
    ImGui::SetColumnWidth(1, 200);
    
    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, (35.0f - 20.0f) / 2));
    PathHistory();
    ImGui::NextColumn();
    ImGui::Columns(1);
    ImGui::EndChild();
    ImGui::PopStyleVar();
  }
  
  void ContentBrowserPanel::SideMenu() {
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, round_factor_);
    ImGui::BeginChild("SideArea", ImVec2(side_child_width_, ImGui::GetWindowHeight() - window_y_offset_),
                      true /* Border */, ImGuiWindowFlags_HorizontalScrollbar);
    
    // Flag for the child menu
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed;
    
    // Shows the content of the side pinned folders
    bool fav_opened = ImGui::TreeNodeEx((void*)"##SizeMenu", flags, "Favourits");
    if (fav_opened) {
      // TODO: Add feature to add pinned path in run time
      int32_t push_id = 0;
      for (auto pinned_path : favourite_paths_) {
        ImGui::PushID(++push_id);

        // Get the filename of the side menu content
        const auto& filename = pinned_path.filename().c_str();
        auto flag = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth
                    | ((pinned_path == current_directory_) ? ImGuiTreeNodeFlags_Selected : 0);
        
        // Show the Pinned filder name
        bool opened = ImGui::TreeNodeEx(filename, flag, filename);
        if (opened) {
          // TODO: Add Function : If opened the menu of current folder then show its content
          ImGui::TreePop();
        }

        if (ImGui::IsItemClicked()) {
          //TODO: For now clear everything
          path_hierarchy_.clear();
          
          // Change current directory
          current_directory_ = pinned_path;
          path_hierarchy_.emplace_back(current_directory_);
        }

        ImGui::PopID();
      }// for (auto pinned_path : pinned_paths_)
      
      ImGui::TreePop();
    } // if (menu_opened)

    
    // Shows the content of the side pinned folders
    bool asset_opened = ImGui::TreeNodeEx((void*)"##SideMenu", flags | ImGuiTreeNodeFlags_DefaultOpen , "Assets");
    if (asset_opened) {
      // TODO: Add feature to add pinned path in run time
      int32_t push_id = 0;
      for (auto pinned_path : asset_paths_) {
        ImGui::PushID(++push_id);
        
        // Get the filename of the side menu content
        const auto& filename = pinned_path.filename().c_str();
        auto flag = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth
        | ((pinned_path == current_directory_) ? ImGuiTreeNodeFlags_Selected : 0);
        
        // Show the Pinned filder name
        bool opened = ImGui::TreeNodeEx(filename, flag, filename);
        if (opened) {
          // TODO: Add Function : If opened the menu of current folder then show its content
          ImGui::TreePop();
        }
        
        if (ImGui::IsItemClicked()) {
          //TODO: For now clear everything
          path_hierarchy_.clear();
          
          // Change current directory
          current_directory_ = pinned_path;
          path_hierarchy_.emplace_back(current_directory_);
        }
        
        ImGui::PopID();
      }// for (auto pinned_path : pinned_paths_)
      
      ImGui::TreePop();
    } // if (menu_opened)
    ImGui::EndChild();
    ImGui::PopStyleVar();
  }
  
  void ContentBrowserPanel::MainArea() {
    static std::shared_ptr<Texture> folder_texture = Renderer::GetTexture(DM::CoreAsset("textures/content_browser/folder.png"));
    static std::shared_ptr<Texture> file_texture = Renderer::GetTexture(DM::CoreAsset("textures/content_browser/file.png"));
    static std::shared_ptr<Texture> jpg_texture = Renderer::GetTexture(DM::CoreAsset("textures/content_browser/jpg.png"));
    static std::shared_ptr<Texture> png_texture = Renderer::GetTexture(DM::CoreAsset("textures/content_browser/png.png"));
    
    static std::shared_ptr<Texture> obj_texture = Renderer::GetTexture(DM::CoreAsset("textures/content_browser/obj.png"));
    static std::shared_ptr<Texture> fbx_texture = Renderer::GetTexture(DM::CoreAsset("textures/content_browser/fbx.png"));
    
    static std::shared_ptr<Texture> font_texture = Renderer::GetTexture(DM::CoreAsset("textures/content_browser/font.png"));
    
    static std::shared_ptr<Texture> cpp_texture = Renderer::GetTexture(DM::CoreAsset("textures/content_browser/cpp.png"));
    static std::shared_ptr<Texture> c_texture = Renderer::GetTexture(DM::CoreAsset("textures/content_browser/c.png"));
    static std::shared_ptr<Texture> h_texture = Renderer::GetTexture(DM::CoreAsset("textures/content_browser/h.png"));

    static std::shared_ptr<Texture> ikan_scene_texture = Renderer::GetTexture(DM::CoreAsset("textures/content_browser/i_kan.png"));
    static std::shared_ptr<Texture> prefab_texture = Renderer::GetTexture(DM::CoreAsset("textures/content_browser/prefab.png"));

    // Push style
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, round_factor_);
    float main_width = ImGui::GetWindowContentRegionWidth() - side_child_width_ - 10;
    ImGui::BeginChild("MainArea", ImVec2(main_width, ImGui::GetWindowHeight() - window_y_offset_),
                      true /* Border */, ImGuiWindowFlags_HorizontalScrollbar);

    // Get the current cursor position of imgui widget
    static ImVec2 init_pos = ImGui::GetCursorPos();

    int32_t directory_idx = 0; // id for each button
    int32_t item_idx = 0; // Item shown each row
    
    int32_t line = 0;
    // Travers entire directory
    for (const auto& directory : std::filesystem::directory_iterator(current_directory_)) {
      // Get the file name from the directory
      const auto& path = directory.path();
      const std::string& filename_string = path.filename().string();

      // If search result passes
      if (search_filter_.PassFilter(filename_string.c_str())) {
        std::shared_ptr<Texture> icon_texture;
        static bool is_directory = false;

        glm::vec2 uv0 = {0.0f, 1.0f};
        glm::vec2 uv1 = {1.0f, 0.0f};

        // Set the icon if current file is a direcotry/folder else use specific file texture
        if (directory.is_directory()) {
          icon_texture = folder_texture;
          is_directory = true;
        }
        else {
          if (".cpp" == path.extension()) icon_texture = cpp_texture;
          else if (".h" == path.extension()) icon_texture = h_texture;
          else if (".c" == path.extension()) icon_texture = c_texture;
          else if (".obj" == path.extension()) icon_texture = obj_texture;
          else if (".fbx" == path.extension()) icon_texture = fbx_texture;
          else if (".ttf" == path.extension()) icon_texture = font_texture;
          else if (saved_scene_extension_ == path.extension()) icon_texture = ikan_scene_texture;
          else if (".png" == path.extension() or ".jpg" == path.extension()  or ".jpeg" == path.extension()) {
            if (texture_image_map_.find(path) == texture_image_map_.end()) {
              texture_image_map_[path] = Renderer::GetTexture(path);
            }
            icon_texture = texture_image_map_.at(path);
          }
          else if (prefab_extenstion_ == path.extension()) {
            if (prefab_image_map_.find(path) == prefab_image_map_.end()) {
              prefab_image_map_[path] = Prefab::GetImageData(path);
            }
            
            ImageData data = prefab_image_map_.at(path);
            if (data.has_data) {
              icon_texture = data.texture;
              uv0 = data.uv0;
              uv1 = data.uv1;
            }
            else {
              icon_texture = prefab_texture;
            }
          }
          else icon_texture = file_texture;
          
          is_directory = false;
        }

        // Icon Button size
        float icon_size_height = std::min(ImGui::GetWindowHeight() - ImGui::GetFontSize() , 64.0f);
        float icon_size_width  = icon_size_height * 0.85;

        ImGui::PushID(filename_string.c_str());
        
        // Update the cursor for each folder/file based on its position
        float cursor_x = ImGui::GetCursorPos().x + ((icon_size_width + 30.0f) * item_idx);
        float cursor_y = init_pos.y + line * 100;
        
        if (cursor_x + icon_size_width > main_width) {
          line++;
          item_idx = 0;
          cursor_x = 8.0f;
          cursor_y = init_pos.y + line * 100;
        }

        ImGui::SetCursorPos(ImVec2(cursor_x, cursor_y));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

        // Render the image button for each folder/file
        bool pressed = PropertyGrid::ImageButton(directory_idx, icon_texture->GetRendererID(), { icon_size_width, icon_size_height }, uv0, uv1);
        
        // If icon is clicked Do some action
        if (pressed) {
          if (is_directory) {
            // Store the current directory in path history
            back_path_history_.emplace_back(current_directory_);

            // Clear the forward path history as we are at the top already
            while (!forward_path_history_.empty()) {
              forward_path_history_.pop_back();
            }

            // Change the current directory
            current_directory_ /= path.filename();
            
            // Store the current path in path hierarchy
            path_hierarchy_.emplace_back(current_directory_);
          } else {
            // DO NOTHING
          }
        } // if pressed

        // Drag the content from here
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
          const std::string& dragged_path = directory.path().string();
          const char* ch = dragged_path.c_str();
          ImGui::SetDragDropPayload("SelectedFile", (void*)ch, dragged_path.size(), ImGuiCond_Always);
          PropertyGrid::ImageButton(directory_idx, icon_texture->GetRendererID(), { 20.0f, 20.0f });
          ImGui::EndDragDropSource();
        }
        ImGui::PopID(); // ImGui::PushID(filename_string.c_str());

        // Text Button : Directory name
        static float wrapWidth = 70.0f;
        ImGui::PushID("CBP_Main_Area_FileNameButton");
        ImGui::SetCursorPos(ImVec2(cursor_x, (init_pos.y + icon_size_height + 10.0f) + line * 100));

        ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrapWidth);
        ImGui::Button(filename_string.c_str(), ImVec2(icon_size_width + 5, 20));
        PropertyGrid::HoveredMsg(filename_string.c_str());
        ImGui::PopTextWrapPos();
        
        ImGui::PopID(); // ImGui::PushID("CBP_Main_Area_FileNameButton");

        ImGui::PopStyleColor(); // ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        
        directory_idx++;
        item_idx++;
      } // search pass
    } // Directory Iterator
    
    ImGui::EndChild();
    ImGui::PopStyleVar();
  }
  
  void ContentBrowserPanel::Back() {
    static std::shared_ptr<Texture> back_texture = Renderer::GetTexture(DM::CoreAsset("textures/icons/back.png"));
    if (PropertyGrid::ImageButton("Back", back_texture->GetRendererID(), icon_size_)) {
      // Do nothing if path history is empty
      if (back_path_history_.empty())
        return;
      
      // Remove the element from path hierarchy
      path_hierarchy_.pop_back();
      
      // Store the current path in forward history
      forward_path_history_.emplace_back(current_directory_);
      
      // Change the current directory and remove element from path history
      current_directory_ = back_path_history_.back();
      back_path_history_.pop_back();
    }
  }
  
  void ContentBrowserPanel::Forward() {
    static std::shared_ptr<Texture> forward_texture = Renderer::GetTexture(DM::CoreAsset("textures/icons/forward.png"));
    if (PropertyGrid::ImageButton("Forward", forward_texture->GetRendererID(), icon_size_)) {
      // Do nothing if path history is empty
      if (forward_path_history_.empty())
        return;
      
      // Store the current path in back history
      back_path_history_.emplace_back(current_directory_);
      
      // Change the current directory and remove element from path history
      current_directory_ = forward_path_history_.back();
      forward_path_history_.pop_back();
      
      // Store the current path in path hierarchy
      path_hierarchy_.emplace_back(current_directory_);
    }
  }
  
  void ContentBrowserPanel::Home() {
    static std::shared_ptr<Texture> home_texture = Renderer::GetTexture(DM::CoreAsset("textures/icons/home.png"));
    if (PropertyGrid::ImageButton("home", home_texture->GetRendererID(), icon_size_)) {
      // Do nothing if current path is same as root
      if (current_directory_ == std::filesystem::path(root_path_)) {
        return;
      }
      
      // Change the current directory
      current_directory_ = root_path_;
      
      // Clear the forward path history as we are at the top already
      forward_path_history_.clear();
      
      // Clear the back path history as we are not considering any back from
      // home for now
      back_path_history_.clear();
      
      // Clear the path hierarchy and add only home path
      path_hierarchy_.clear();
      path_hierarchy_.emplace_back(current_directory_);
    }
  }
  
  void ContentBrowserPanel::Search() {
    ImGui::SetNextItemWidth(ImGui::GetFontSize() * 16);
    if (PropertyGrid::Search(search_filter_.InputBuf, "Search ... ", icon_size_)) {
      search_filter_.Build();
    }
  }
  
  void ContentBrowserPanel::PathHistory() {
    size_t i = 0;
    for (const auto& path : path_hierarchy_) {
      i++;
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
      if (ImGui::Button(path.filename().c_str())) {
      }
      ImGui::PopStyleColor();
      ImGui::SameLine();
      
      if (i != path_hierarchy_.size())
        ImGui::Text(">");
      ImGui::SameLine();
    }
  }

}// namespace ikan
