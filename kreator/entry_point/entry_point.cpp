//
//  main.cpp
//  kreator
//
//  Created by Ashish . on 27/03/23.
//

#include <iostream>

void EventHandler(ikan::Event& event) {
  
}

int main() {
  // Initialize the ikan Logger
  auto core_level   = ikan::Logger::Level::Trace;
  auto client_level = ikan::Logger::Level::Trace;
  
  ikan::Logger::Init(core_level, client_level, "../../../log/kreator.log");
  
#ifdef IK_DEBUG_FEATURE
  auto spd_core_log_level = ikan::Logger::GetSpdLevelFromIKanLevel(core_level);
  auto spd_client_log_level = ikan::Logger::GetSpdLevelFromIKanLevel(client_level);
  
  IK_INFO("Core Entry Point", "Initialized the spd logger ");
  IK_INFO("Core Entry Point", "  Core   | {0}", ikan::Logger::GetLogLevelStringFromSpdLevel(spd_core_log_level));
  IK_INFO("Core Entry Point", "  Client | {0}", ikan::Logger::GetLogLevelStringFromSpdLevel(spd_client_log_level));
#endif
  
  ikan::Window::Specification window_specification;
  
  window_specification.title = "Untitled Window";
  window_specification.width = 2100;
  window_specification.height = 900;
  window_specification.v_sync = true;
  window_specification.fullscreen = false;
  window_specification.hide_titlebar = false;
  
  std::unique_ptr<ikan::Window> window_;
  {
    ikan::Renderer::CreateRendererData(ikan::Renderer::Api::OpenGl);
    
    window_ = ikan::Window::Create(ikan::OperatingSystem::Mac, window_specification);
    window_->SetEventFunction(std::bind(EventHandler, std::placeholders::_1));
    
    ikan::Renderer::Initialize();

    IK_CORE_INFO(ikan::LogModule::None, "--------------------------------------------------------------------------");
    IK_CORE_INFO(ikan::LogModule::None, "                     Core Application Initialized                         ");
    IK_CORE_INFO(ikan::LogModule::None, "--------------------------------------------------------------------------");
  }

  {
    IK_CORE_INFO(ikan::LogModule::None, "--------------------------------------------------------------------------");
    IK_CORE_INFO(ikan::LogModule::None, "                          Starting Game Loop                              ");
    IK_CORE_INFO(ikan::LogModule::None, "--------------------------------------------------------------------------");

    while (window_->IsActive()) {
      window_->Update();
    }
    
    IK_CORE_INFO(ikan::LogModule::None, "--------------------------------------------------------------------------");
    IK_CORE_INFO(ikan::LogModule::None, "                            Ending Game Loop                              ");
    IK_CORE_INFO(ikan::LogModule::None, "--------------------------------------------------------------------------");
  }
  {
    ikan::Renderer::Shutdown();
  }
  
  return 0;
}
