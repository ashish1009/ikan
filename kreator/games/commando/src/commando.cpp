//
//  commando.cpp
//  kreator
//
//  Created by Ashish . on 08/05/23.
//

#include "commando.hpp"

namespace commando {
  
  Commando::Commando() {
    Batch2DRenderer::AddQuadData(2000);
  }
  
  Commando::~Commando() {
  }
  
  void Commando::Init(const std::shared_ptr<Scene> scene, Viewport* viewport) {
    Batch2DRenderer::AddQuadData(2000);

    scene_ = scene;
    viewport_ = viewport;
    
#if 1
    auto tag_view = scene_->GetEntitesWith<QuadComponent>();
    for (auto entity : tag_view) {
      Entity e = Entity(entity, scene_.get());
      auto& t = e.GetComponent<TagComponent>().tag;
      if (t.find("woodenSupportsBlock") != std::string::npos) {
        if (e.HasComponent<Box2DColliderComponent>()) {
          e.RemoveComponent<Box2DColliderComponent>();
          e.RemoveComponent<RigidBodyComponent>();
//          auto& b = e.GetComponent<Box2DColliderComponent>();
//          b.size = {0.2, 0.1};
//          b.offset = {0.0, -0.65};
        }
      }
    }
#endif
  };

  void Commando::AddQuadFromTexture(const std::string& path) {
    const auto& cam_data = scene_->GetPrimaryCameraData();
    if (!cam_data.scene_camera) return;
    
    if (cam_data.scene_camera->GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
      float zoom = viewport_->height / cam_data.scene_camera->GetZoom();
      float x_pos = (((viewport_->mouse_pos_x - (float)viewport_->width / 2) / zoom) + cam_data.position.x);
      float y_pos = (((viewport_->mouse_pos_y - (float)viewport_->height / 2) / zoom) + cam_data.position.y);

      x_pos += 0.5f; // Adding 0.5 offset as camera starts from 0
      y_pos += 0.5f; // Adding 0.5 offset as camera starts from 0

      x_pos = std::floor(x_pos);
      y_pos = std::floor(y_pos);

      std::string name = StringUtils::GetNameFromFilePath(path);
      Entity e = scene_->CreateEntity(name);
      auto& tc = e.GetComponent<TransformComponent>();
      tc.UpdateScale(Y, 2);
      
      tc.UpdatePosition(X, x_pos);
      tc.UpdatePosition(Y, y_pos + 0.75f);
      tc.UpdatePosition(Z, -1 * ((tc.Position().y / 0.25) * 0.001f));
      
      auto& qc = e.AddComponent<QuadComponent>();
      qc.sprite.texture.push_back(Renderer::GetTexture(path));
    }
  }
  
  void Commando::Update(Timestep ts) {

  }

  void Commando::MoveEntities(Direction direction, const std::unordered_map<entt::entity, Entity*>& selected_entities) {
    for (auto& [entt, entity] : selected_entities) {
      if(!entity) continue;
      
      auto& tc = entity->GetComponent<TransformComponent>();
      glm::vec2 iso_step;
      switch (direction) {
        case Down:
          iso_step = Math::GetIsometricFromCartesian({0, -0.5});
          tc.AddPosition(Z, 0.001);
          break;
        case Up:
          iso_step = Math::GetIsometricFromCartesian({0, 0.5});
          tc.AddPosition(Z, -0.001);
          break;
        case Right:
          iso_step = Math::GetIsometricFromCartesian({0.5, 0});
          tc.AddPosition(Z, -0.001);
          break;
        case Left:
          iso_step = Math::GetIsometricFromCartesian({-0.5, 0});
          tc.AddPosition(Z, 0.001);
          break;
        default: break;
      }
      
      tc.AddPosition(X, iso_step.x);
      tc.AddPosition(Y, iso_step.y);
    }
  }

} // namespace commando
