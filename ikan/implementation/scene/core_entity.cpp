//
//  entity.cpp
//  ikan
//
//  Created by Ashish . on 10/04/23.
//

#include "core_entity.hpp"
#include "components.hpp"
#include "scene_serialiser.hpp"

namespace ikan {
    
  Entity::Entity(entt::entity handle, Scene* scene)
  : entity_handle_(handle), scene_(scene) {
    IK_CORE_TRACE(LogModule::Entity, "Creating Entity with Handle: {0} ...", (uint32_t)entity_handle_);
  }
  Entity::Entity(Entity&& other) : entity_handle_(other.entity_handle_),
  scene_(other.scene_) {
    IK_CORE_ASSERT(IsValidScene(), "Scene is invalid, Entity will be problematic !!!");
    IK_CORE_TRACE(LogModule::Entity, "Moving Entity with Handle: {0} ...", (uint32_t)entity_handle_);
  }
  Entity::Entity(const Entity& other) : entity_handle_(other.entity_handle_),
  scene_(other.scene_) {
    IK_CORE_ASSERT(IsValidScene(), "Scene is invalid, Entity will be problematic !!!");
    IK_CORE_TRACE(LogModule::Entity, "Copying Entity with Handle: {0} ...", (uint32_t)entity_handle_);
  }
  Entity& Entity::operator=(const Entity& other) {
    IK_CORE_ASSERT(IsValidScene(), "Scene is invalid, Entity will be problematic !!!");
    entity_handle_ = other.entity_handle_;
    scene_ = other.scene_;
    IK_CORE_TRACE(LogModule::Entity, "Copying Entity using = operator with Handle {0} ...", (uint32_t)entity_handle_);
    return *this;
  }
  Entity& Entity::operator=(Entity&& other) {
    IK_CORE_ASSERT(IsValidScene(), "Scene is invalid, Entity will be problematic !!!");
    entity_handle_ = other.entity_handle_;
    scene_ = other.scene_;
    IK_CORE_TRACE(LogModule::Entity, "Moving Entity using = operator with Handle {0} ...", (uint32_t)entity_handle_);
    return *this;
  }
  Entity::~Entity() {
    IK_CORE_TRACE(LogModule::Entity, "Destroying Entity with Handle: {0} !!!", (uint32_t)entity_handle_);
  }

  bool Entity::IsValidScene() const {
    return scene_ and StringUtils::GetExtensionFromFilePath(scene_->GetFilePath()) == saved_scene_extension_;
  }
  
} // namespace ikan