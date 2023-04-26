//
//  texture_component.h
//  ikan
//
//  Created by Ashish . on 20/04/23.
//

#pragma once

#include "renderer/graphics/texture.hpp"

namespace ikan {
  
  static inline bool LoadTextureIcon(std::shared_ptr<Texture>& texture) {
    bool texture_changed = false;
    
    static std::shared_ptr<Texture> no_texture = Renderer::GetTexture(DM::CoreAsset("textures/default/no_texture.png"));
    size_t tex_id = ((texture) ? texture->GetRendererID() : no_texture->GetRendererID());
    
    // Show the image of texture
    ImGui::Image((void*)tex_id, ImVec2(40.0f, 40.0f), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
    
    // Drop the texture here and load new texture
    PropertyGrid::DropConent([&](const std::string& path)
                             {
      texture.reset();
      texture = Renderer::GetTexture(path);
      texture_changed = true;
    });
    PropertyGrid::HoveredMsg("Drop the Texture file in the Image Button to upload the texture");
    return texture_changed;
  }
  
  /// This structure holds the texture component with use flag
  struct TextureComponent {
    bool use = false;
    float tiling_factor = 1.0f;
    std::shared_ptr<Texture> texture = nullptr;
    
    TextureComponent(const std::shared_ptr<Texture>& tex = nullptr, bool use = true);
    DEFINE_COPY_MOVE_CONSTRUCTORS(TextureComponent)
    
    template<typename UIFunction>
    /// This function renders the texture components
    /// - Parameters:
    ///   - color: color of the texture
    ///   - ui_function: function to render below texture Use
    void RenderGui(glm::vec4& color, UIFunction ui_function) {
      ImGui::Columns(2);
      ImGui::SetColumnWidth(0, 60);
      
      LoadTextureIcon(texture);
      ImGui::NextColumn();
      
      // Check box to togle use of texture
      ImGui::Checkbox("Use ", &use);
      if (use and texture) {
        ImGui::DragFloat("", &tiling_factor, 1.0f, 1.0f, 1000.0f);
        PropertyGrid::HoveredMsg("Tiling Factor");
      }
      ui_function();
      ImGui::Columns(1);
    }
  };
  
  /// This structure holds the property of sprite component (Sub Texture)
  struct SpriteComponent : public TextureComponent {
    enum class Type { Sprite = 0, Animation = 1 };

    bool linear_edge = true;
    bool use_sub_texture = false;
    
    // Animation Sprite Data
    int32_t speed = 15;
    int32_t anim_idx = 0; // No need to copy or save in scene. always starts from 0
    std::vector<std::shared_ptr<SubTexture>> sprite_images;
    
    void ClearSprites() { sprite_images.clear(); }
    void ResetAnimIndx() {
      if (anim_idx >= speed * sprite_images.size() or anim_idx < 1)
        anim_idx = 0;
    }
    
    SpriteComponent(const std::shared_ptr<Texture>& comp = nullptr, bool use = true);
    ~SpriteComponent();
    DEFINE_COPY_MOVE_CONSTRUCTORS(SpriteComponent)
    
    template<typename UIFunction>
    /// This function renders the texture components
    /// - Parameters:
    ///   - color: color of the texture
    ///   - ui_function: function to render below texture Use
    void RenderGui(glm::vec4& color, UIFunction ui_function) {
      ImGui::Columns(2);
      ImGui::SetColumnWidth(0, 60);
      
      if (LoadTextureIcon(texture)) {
        LoadTexture(texture);
      }
      ImGui::NextColumn();
      
      // Check box to togle use of texture
      ImGui::Checkbox("Use ", &use);
      PropertyGrid::HoveredMsg("Enable to Render the Sprite out the Texture");
      if (use and texture) {
        ImGui::SameLine();
        if (ImGui::Checkbox("Linear Edge", &linear_edge)) {
          ChangeLinearTexture();
        }
        PropertyGrid::HoveredMsg("Enable to Render the Sprite out the Texture");
        
        ImGui::SameLine();
        ImGui::Checkbox("Sprite", &use_sub_texture);
        PropertyGrid::HoveredMsg("Enable to Render the Sprite out the Texture");
      }
      ui_function();
      
      if (use) {
        ImGui::SameLine();
        ImGui::DragFloat("", &tiling_factor, 1.0f, 1.0f, 1000.0f);
        PropertyGrid::HoveredMsg("Tiling Factor");
        ImGui::Columns(1);
        
        // Selection of type Animation or Sprite
        ImGui::Separator();
        
        if (use_sub_texture) {
          SubtextureGui();
        } // if Sub texture
      } // If Use Texture
      
      ImGui::Columns(1);
    }
    
  private:
    /// This function loads the textrue and sprite again
    /// - Parameter other: component
    void LoadTexture(const SpriteComponent& other);
    /// This function changes the linear flag of texture
    void ChangeLinearTexture();

    /// This function Renders SubTexture Gui
    void SubtextureGui();
    /// This function Renders Sprite Gui
    void SpriteGui();
    /// This function Renders Animatiom Gui
    void AnimationGui();
  };
  
} // namespace ikan
