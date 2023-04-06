//
//  renderer_layer.hpp
//  kreator
//
//  Created by Ashish . on 05/04/23.
//

#pragma once

#include "game_data.hpp"

namespace kreator {
  
  using namespace ikan;
  
  class RendererLayer : public Layer {
  public:
    struct SettingWrapper {
      std::string name;
      bool flag = true;
      SettingWrapper(const std::string& name, bool flag) : name(name), flag(flag) {}
      
      void ShowInMenu() {
        ImguiAPI::MenuItem(name.c_str(), nullptr, flag, true, [this]() {
          flag = (flag) ? false : true;
        });
      }
      void CheckBox() {
        PropertyGrid::CheckBox(name.c_str(), flag, 3 * ImGui::GetWindowContentRegionMax().x / 4);
      }
    };

    struct Setting {
      SettingWrapper frame_rate{"Frame Rate", true};
      SettingWrapper common_renderer_stats{"Common Renderer Stats", true};
      SettingWrapper renderer_stats_2d{"2D Renderer Stats", true};
    };
    
    /// Layer Default Constructor to store the name of layer
    /// - Parameter game_type: Type of game to render using this layer
    RendererLayer(GameType game_type);
    /// Default Destructor
    ~RendererLayer();
    
    /// This funciton initialises the layer, calls when we push layer in the application (To be called only once)
    void Attach() override;
    /// This funciton destroy the layer, calls when we pop layer from the application (To be called only once)
    void Detach() override;
    
    /// This function renders the GUI Window for this layer. To be called each frame from application.
    /// - Note: Core Application is taking care to call the RenderGui API for all Layers
    void RenderGui() override;
    /// This function updates the layer data. To be called each frame from application.
    /// - Note Core Application is taking care to call the Update(ts) API for all Layers
    /// - Parameter ts: Time step between 2 Frames
    void Update(Timestep ts) override;
    /// This function handles all the events triggered for the layer. To be whenever an event is triggered in the window.
    /// - Note Core Application is taking care to call the EventHandler(event) API for all Layers
    /// - Parameter event: Event (Base class) intance. Dispatch event from Event Dispatcher
    void HandleEvents(Event& event) override;
    
  private:
    /// This function render the menue bar
    void ShowMenu();
    /// This function renders the setting Widget
    void ShowSettings();

    // Member variables
    bool is_playing_ = false;
    std::unique_ptr<GameData> game_data_;
    Setting setting_;
  };
  
} // namespace kreator