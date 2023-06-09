//
//  scriptable_entity.hpp
//  ikan
//
//  Created by Ashish . on 21/04/23.
//

#pragma once

#include "scene/core_entity.hpp"

#include <box2d/b2_contact.h>

namespace ikan {
  
#define ScriptLoader(x, ...) \
  [=](NativeScriptComponent* sc, const std::string& script_name) {  \
    if (script_name == #x) {                                        \
      sc->Bind<x>(__VA_ARGS__);                                     \
      return true;                                                  \
    }                                                               \
    return false;                                                   \
  }
  
  class NativeScriptComponent;
  class RigidBodyComponent;

  // Using Typedefs
  using ScriptLoaderFn = std::function<bool(NativeScriptComponent* sc, const std::string& script_name)>;

  /// This file holds all the native scripts. For now we are using only C type Scripts so we have to pre define all of them. No Runtime script is supported
  /// yet. So store all the script name as their class name in ScriptManager::scripts_
  class ScriptManager {
  public:
    static std::vector<std::string> scripts_;
    
    /// This function update the scrip bind function pointer
    /// - Parameters:
    ///   - sc: Native script component pointer
    ///   - script_name: Script name
    static void UpdateScript(NativeScriptComponent* sc, const std::string& script_name, ScriptLoaderFn loader_function);
    
    MAKE_PURE_STATIC(ScriptManager);
  };
  
  class ScriptableEntity {
  public:
    template <typename... Args>
    ScriptableEntity(Args... args) {}
    virtual ~ScriptableEntity() {}
    
    template<typename T> T& GetComponent() { return entity_.GetComponent<T>(); }
    template<typename T> bool HasComponent() { return entity_.HasComponent<T>(); }
    
    virtual void EventHandler(Event& event) {}
    virtual void RenderGui() {}
    virtual void BeginCollision(Entity* collided_entity, b2Contact* contact, const glm::vec2& contact_normal) {}
    virtual void EndCollision(Entity* collided_entity, b2Contact* contact, const glm::vec2& contact_normal) {}
    virtual void PreSolve(Entity* collided_entity, b2Contact* contact, const glm::vec2& contact_normal) {}
    virtual void PostSolve(Entity* collided_entity, b2Contact* contact, const glm::vec2& contact_normal) {}
    virtual void Copy(void* script) {}
    
  protected:
    virtual void Create(Entity entity) {
      entity_ = entity;
    }
    virtual void Destroy() {}
    virtual void Update(Timestep ts) {}
    
  protected:
    Entity entity_;
    Scene* scene_;
    friend class Scene;
  };
  
} // namespace ikan
