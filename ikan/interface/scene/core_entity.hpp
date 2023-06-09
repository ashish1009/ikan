//
//  entity.hpp
//  ikan
//
//  Created by Ashish . on 10/04/23.
//

#pragma once

#include "scene/scene.hpp"

namespace ikan {
  
  /// enTT Wrapper class to get, add .. APIs
  struct Entity {
    /// This Constructor creates an Entity with handle and scene pointer
    /// - parameters:
    ///   - handle: handle of Entity. Scene (registry) takes care for this
    ///   - scene: Scene pointer to which this entity to be bound
    Entity(entt::entity handle = { entt::null }, Scene* scene = nullptr);
    /// This is default entity Destrcutor
    ~Entity();
    
    /// This function checks is Scene is valid
    bool IsValidScene() const;

    /// This function returns the UUID of Current entity
    UUID GetUUID() const;
    /// This function returns name of entity
    const std::string& GetName() const;
    
    /// This function returns Entity as entity handle
    operator entt::entity() const { return entity_handle_; }
    /// This function returns the entity handle from this Entity
    operator uint32_t() const { return (uint32_t)entity_handle_; }
    /// This function returns entity as Boolean Check if entity handle is not null
    operator bool() const { return entity_handle_ != entt::null; }
    /// This function returns if this entity is equal to other
    /// - parameter other: other entity
    bool operator ==(const Entity& other) const { return entity_handle_ == other.entity_handle_ and scene_ == other.scene_; }
    /// This function returns if this entity is not equal to other
    /// - parameter other: other entity
    bool operator !=(const Entity& other) const { return !(*this == other); }
    /// This function returns if this entity is equal to other (only handle)
    /// - parameter other: other entity handle
    bool operator ==(const entt::entity& other) const { return entity_handle_ == other; }
    /// This function returns if this entity is not equal to other (only handle)
    /// - parameter other: other entity handle
    bool operator !=(const entt::entity& other) const { return !(*this == other); }
    
    /// This function adds component in Current Entity
    /// - Parameter args: Arguments needed to construct the component NOTE : T is type of component
    template<typename T, typename... Args> T& AddComponent(Args&&... args) {
      IK_ASSERT(!HasComponent<T>(), "Entity already has component!");
      return scene_->registry_.emplace<T>(entity_handle_, std::forward<Args>(args)...);
    }
    
    /// This function returns component from Current Entity NOTE : T is type of component
    template<typename T> [[nodiscard("Return value discarded")]] T& GetComponent() const {
      IK_ASSERT(HasComponent<T>(), "Entity does not have component!");
      return scene_->registry_.get<T>(entity_handle_);
    }
    
    /// This function adds component in Current Entity (replace if already present)
    /// - Parameter args: Arguments needed to construct the component
    /// - Note : T is type of component
    template<typename T, typename... Args> T& AddOrReplaceComponent(Args&&... args) {
      T& component = scene_->registry_.emplace_or_replace<T>(entity_handle_, std::forward<Args>(args)...);
      return component;
    }
    
    /// This function checks if Entity have a component return true if present NOTE : T is type of component
    template<typename T> bool HasComponent() const {
      return scene_->registry_.has<T>(entity_handle_);
    }
    
    /// This function removes component from Current Entity NOTE : T is type of component
    template<typename T> void RemoveComponent() const {
      IK_ASSERT(HasComponent<T>(), "Entity does not have component!");
      scene_->registry_.remove<T>(entity_handle_);
    }
    
    /// This function checks the entity e is valid or not
    /// - Parameter e: entity to be checked
    static bool IsValid(Entity* e);
    
    DEFINE_COPY_MOVE_CONSTRUCTORS(Entity);

    entt::entity entity_handle_{ entt::null };
    Scene* scene_ = nullptr;
  };
  
} // namespace ikan
