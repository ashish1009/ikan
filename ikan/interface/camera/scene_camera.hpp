//
//  scene_camera.hpp
//  ikan
//
//  Created by Ashish . on 07/04/23.
//

#pragma once

#include "camera/camera.hpp"

namespace ikan {
  
  /// Scene Camera class to store the projection for active scene
  class SceneCamera : public Camera {
  public:
    /// Projection type of Scene Camera
    /// - Perspective for 3D
    /// - Orthographics for 2D
    enum class ProjectionType {
      Perspective = 0, Orthographic = 1
    };
        
    /// This constructor creates the Scene Camera instance
    /// - Parameter proj_type: Projection type
    SceneCamera(ProjectionType proj_type = ProjectionType::Orthographic);
    /// Default Scene Camera Desctructorr
    virtual ~SceneCamera();
    
    /// This function updates the Near plane of camera
    /// - Parameter near: New near plane
    void SetNear(float near) override;
    /// This function updates the Far plane of camera
    /// - Parameter far: new fat plane
    void SetFar(float far) override;
    /// This function returns the zoom value of camera
    float GetZoom() const override;
    
    /// This function renders Imgui pannel for Scene should cal this function between Imgui::Begin and Imgui::End
    void RenderGui();
    /// This function renders Imgui pannel for Scene Camera
    void RenderGuiWithTitle();
    
    /// This function renders the grid
    /// - Parameters:
    ///   - max_lines: Max line to render in grids
    ///   - camera_transform: Camera transform from component
    ///   - camera_pos: Camera position
    void RenderGrids(uint32_t max_lines, const glm::mat4& camera_transform, const glm::vec3 camera_pos);
    
    /// This function updates the new Orthographic Matrix (Only for Orthographic camera)
    /// - Parameters:
    ///   - size: new size of Camera
    ///   - near_clip: new near clip
    ///   - far_clip: new far clip
    void SetOrthographic(float size, float near_clip, float far_clip);
    /// This function updates the new Perspective Matrix (Only for Perspective camera)
    /// - Parameters:
    ///   - fov: new FOV
    ///   - near_clip: new near clip
    ///   - far_clip: new far clip
    void SetPerspective(float fov, float near_clip, float far_clip);
    
    /// This function updates the Viewport size of camera
    /// - Parameters:
    ///   - width: new width
    ///   - height: new height
    void SetViewportSize(uint32_t width, uint32_t height);
    
    /// This function updates the Camera projection type
    /// - Parameter type: new projection type
    void SetProjectionType(ProjectionType type);
    /// This function updates the Size of Orthographic Camera (Only for Orthographic Camera)
    /// - Parameter size: new size
    void SetOrthographicSize(float size);
    /// This function updates FOV of Perspective Camera (Only for Perspective camera)
    /// - Parameter fov: new FOV
    void SetPerspectiveFOV(float fov);
    
    /// This function renders the widget for zoom
    /// - Note: Should called between Begin and End
    void ZoomWidget();
    
    /// This function returns the Projection type of camera
    ProjectionType GetProjectionType() const;
    /// This function returns size of Orthographic Camera
    float GetOrthographicSize() const;
    /// This function returns FOV of Perspective Camera
    float GetPerspectiveFOV() const;
    
    DEFINE_COPY_MOVE_CONSTRUCTORS(SceneCamera);

    // Debug data
    // No need to make private
    bool grid_2d_ = true;
    bool isometric_ = false;

  private:
    // Member functions
    /// This function recalculate the projection matrix
    void RecalculateProjection();
    
    // Member variables
    ProjectionType projection_type_ = ProjectionType::Orthographic;
    float perspective_fov_ = glm::radians(75.0f);
    float orthographic_size_ = 10.0f;
  };
  
} // namespace ikan
