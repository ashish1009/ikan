//
//  scene_camera.cpp
//  ikan
//
//  Created by Ashish . on 07/04/23.
//

#include "scene_camera.hpp"
#include "property_grid.hpp"
#include "renderer/utils/batch_2d_renderer.hpp"

namespace ikan {
  
  namespace camera_utils {
#ifdef IK_DEBUG_FEATURE
    /// This function returns the Projection type in string
    /// - Parameter proj_type: Projection type
    static std::string GetProjectionNameFromType(SceneCamera::ProjectionType proj_type) {
      switch (proj_type) {
        case SceneCamera::ProjectionType::Orthographic: return "Orthographic";
        case SceneCamera::ProjectionType::Perspective: return "Perspective";
        default: IK_ASSERT(false, "Invalid Projection Type");
      }
    }
#endif
  } // namespace camera_utils
  
  SceneCamera::SceneCamera(ProjectionType proj_type)
  : projection_type_(proj_type) {
    IK_CORE_TRACE(LogModule::SceneCamera, "Creating Scene Camera ...");
    IK_CORE_TRACE(LogModule::SceneCamera, "  Projection Type  {0}", camera_utils::GetProjectionNameFromType(projection_type_));
    
    // Update the variables based on the projection type of camera
    if (projection_type_ == ProjectionType::Orthographic) {
      far_plane_ = 1.0f;
      near_plane_ = -1.0f;
      
      IK_CORE_TRACE(LogModule::SceneCamera, "  Size             {0}", orthographic_size_);
    }
    else if (projection_type_ == ProjectionType::Perspective) {
      far_plane_ = 10000.0f;
      near_plane_ = 0.01f;
      
      IK_CORE_TRACE(LogModule::SceneCamera, "  FOV              {0}", perspective_fov_);
    }
    else {
      IK_ASSERT(false, "Invalid Projection Type");
    }
    
    IK_CORE_TRACE(LogModule::SceneCamera, "  Aspect Ratio     {0}", aspect_ratio_);
    IK_CORE_TRACE(LogModule::SceneCamera, "  Near Plane       {0}", near_plane_);
    IK_CORE_TRACE(LogModule::SceneCamera, "  Far Plane        {0}", far_plane_);
    
    // Recalculate the projection matrix
    RecalculateProjection();
  }
  
  SceneCamera::~SceneCamera() {
    IK_CORE_TRACE(LogModule::SceneCamera, "Destroying Scene Camera !!!");
    IK_CORE_TRACE(LogModule::SceneCamera, "  Projection Type  {0}", camera_utils::GetProjectionNameFromType(projection_type_));
    
    if (projection_type_ == ProjectionType::Orthographic) {
      IK_CORE_TRACE(LogModule::SceneCamera, "  Size             {0}", orthographic_size_);
    }
    else if (projection_type_ == ProjectionType::Perspective) {
      IK_CORE_TRACE(LogModule::SceneCamera, "  FOV              {0}", perspective_fov_);
    }
    else {
      IK_ASSERT(false, "Invalid Projection Type");
    }
    IK_CORE_TRACE(LogModule::SceneCamera, "  Aspect Ratio     {0}", aspect_ratio_);
    IK_CORE_TRACE(LogModule::SceneCamera, "  Near Plane       {0}", near_plane_);
    IK_CORE_TRACE(LogModule::SceneCamera, "  Far Plane        {0}", far_plane_);
  }
  
  void SceneCamera::RecalculateProjection() {
    // Update the projection matrix based on the projection type of camera
    if (projection_type_ == ProjectionType::Perspective) {
      projection_matrix_ = glm::perspective(perspective_fov_, aspect_ratio_, near_plane_, far_plane_);
    }
    else if (projection_type_ == ProjectionType::Orthographic) {
      float ortho_left = -orthographic_size_ * aspect_ratio_ * 0.5f;
      float ortho_right = orthographic_size_ * aspect_ratio_ * 0.5f;
      float ortho_bottom = -orthographic_size_ * 0.5f;
      float ortho_top = orthographic_size_ * 0.5f;
      
      projection_matrix_ = glm::ortho(ortho_left, ortho_right, ortho_bottom, ortho_top, near_plane_, far_plane_);
    }
    else {
      IK_ASSERT(false, "Invalid Projection Type");
    }
  }
  
  
  SceneCamera& SceneCamera::operator=(const SceneCamera &other) {
    grid_2d_ = other.grid_2d_;
    near_plane_ = other.near_plane_;
    far_plane_ = other.far_plane_;
    aspect_ratio_ = other.aspect_ratio_;
    
    projection_matrix_ = other.projection_matrix_;
    
    projection_type_ = other.projection_type_;
    perspective_fov_ = other.perspective_fov_;
    orthographic_size_ = other.orthographic_size_;
    
    return *this;
  }
  
  void SceneCamera::SetNear(float near) {
    near_plane_ = near;
    IK_CORE_TRACE(LogModule::SceneCamera, "New Near Plane for Scene Camera : {0}. (NOTE: Updating View Projection Matrix)", near_plane_);
    
    // Recalculate the projection matrix
    RecalculateProjection();
  }
  
  void SceneCamera::SetFar(float far) {
    far_plane_ = far;
    IK_CORE_TRACE(LogModule::SceneCamera, "New Far Plane for Scene Camera : {0}. (NOTE: Updating View Projection Matrix)", far_plane_);
    
    // Recalculate the projection matrix
    RecalculateProjection();
  }
  
  float SceneCamera::GetZoom() const {
    if (projection_type_ == ProjectionType::Orthographic) {
      return orthographic_size_;
    }
    else if (projection_type_ == ProjectionType::Perspective) {
      return perspective_fov_;
    }
    IK_CORE_ASSERT(false, "Invalid Type");
  }
  
  void SceneCamera::SetOrthographic(float size, float near_clip, float far_clip) {
    projection_type_ = ProjectionType::Orthographic;
    orthographic_size_ = size;
    near_plane_ = near_clip;
    far_plane_ = far_clip;
    
    IK_CORE_TRACE(LogModule::SceneCamera, "Updating Orthograpic Data of Scene Camera");
    IK_CORE_TRACE(LogModule::SceneCamera, "  Size             {0}", orthographic_size_);
    IK_CORE_TRACE(LogModule::SceneCamera, "  Aspect Ratio     {0}", aspect_ratio_);
    IK_CORE_TRACE(LogModule::SceneCamera, "  Near Plane       {0}", near_plane_);
    IK_CORE_TRACE(LogModule::SceneCamera, "  Far Plane        {0}", far_plane_);
    IK_CORE_TRACE(LogModule::SceneCamera, "Updating View Porjection Matrix");
    
    // Recalculate the projection matrix
    RecalculateProjection();
  }
  
  void SceneCamera::SetPerspective(float fov, float near_clip, float far_clip) {
    projection_type_ = ProjectionType::Perspective;
    perspective_fov_ = fov;
    near_plane_ = near_clip;
    far_plane_ = far_clip;
    
    IK_CORE_TRACE(LogModule::SceneCamera, "Updating Perspective Data of Scene Camera");
    IK_CORE_TRACE(LogModule::SceneCamera, "  FOV              {0}", perspective_fov_);
    IK_CORE_TRACE(LogModule::SceneCamera, "  Aspect Ratio     {0}", aspect_ratio_);
    IK_CORE_TRACE(LogModule::SceneCamera, "  Near Plane       {0}", near_plane_);
    IK_CORE_TRACE(LogModule::SceneCamera, "  Far Plane        {0}", far_plane_);
    IK_CORE_TRACE(LogModule::SceneCamera, "Updating View Porjection Matrix");
    
    // Recalculate the projection matrix
    RecalculateProjection();
  }
  
  void SceneCamera::SetViewportSize(uint32_t width, uint32_t height) {
    aspect_ratio_ = (float)width / (float)height;

    // Recalculate the projection matrix
    RecalculateProjection();
  }
  
  /// Getters
  SceneCamera::ProjectionType SceneCamera::GetProjectionType() const {
    return projection_type_;
  }
  float SceneCamera::GetOrthographicSize() const {
    return orthographic_size_;
  }
  float SceneCamera::GetPerspectiveFOV() const {
    return perspective_fov_;
  }
  
  void SceneCamera::SetProjectionType(SceneCamera::ProjectionType type) {
    IK_CORE_TRACE(LogModule::SceneCamera, "Changing Projection Type of Scene Camera : {0}. (NOTE: Updating View Projection Matrix)",
                 camera_utils::GetProjectionNameFromType(projection_type_));
    
    projection_type_ = type;
    // Update the parameters based on the projection type of camera
    if (projection_type_ == ProjectionType::Orthographic) {
      far_plane_ = 1.0f;
      near_plane_ = -1.0f;
    }
    else if (projection_type_ == ProjectionType::Perspective) {
      far_plane_ = 10000.0f;
      near_plane_ = 0.01f;
    }
    else {
      IK_ASSERT(false, "Invalid Projection Type");
    }
    
    // Recalculate the projection matrix
    RecalculateProjection();
  }
  
  void SceneCamera::SetOrthographicSize(float size) {
    IK_CORE_TRACE(LogModule::SceneCamera, "Changing Orthographic Size of Scene Camera : {0}. (NOTE: Updating View Projection Matrix)", size);
    orthographic_size_ = size;
    
    // Recalculate the projection matrix
    RecalculateProjection();
  }
  
  void SceneCamera::SetPerspectiveFOV(float fov) {
    IK_CORE_TRACE(LogModule::SceneCamera, "Changing Perspective FOV of Scene Camera : {0}. (NOTE: Updating View Projection Matrix)", fov);
    perspective_fov_ = fov;
    
    // Recalculate the projection matrix
    RecalculateProjection();
  }
  
  void SceneCamera::RenderGui() {
    ProjectionType new_proj_type = ProjectionType(PropertyGrid::ComboDrop("Projection Type", { "Perspective" , "Orthographic" },
                                                                          (uint32_t)projection_type_));
    
    // Render the property based on the projection type of camera
    if (new_proj_type != projection_type_)
      SetProjectionType(new_proj_type);
    
    if (projection_type_ == SceneCamera::ProjectionType::Perspective) {
      float fov = glm::degrees(perspective_fov_);
      if (PropertyGrid::Float1("Vertical FOV", fov, nullptr, 1.0f, 45.0f)) {
        SetPerspectiveFOV(glm::radians(fov));
      }
      PropertyGrid::HoveredMsg(std::to_string(perspective_fov_).c_str());
      
    } else if (projection_type_ == SceneCamera::ProjectionType::Orthographic) {
      ZoomWidget();
      ImGui::Separator();
      PropertyGrid::CheckBox("Grids", grid_2d_);
      if (grid_2d_) {
        PropertyGrid::CheckBox("Isometric ", isometric_);
      }
    } else {
      IK_ASSERT(false, "Invalid Projection Type");
    }
  }
  
  void SceneCamera::RenderGuiWithTitle() {
    ImGui::Begin("Scene Camera");
    ImGui::PushID("Scene Camera");
    
    RenderGui();
    
    ImGui::Separator();
    ImGui::PopID();
    ImGui::End();
  }
  
  void SceneCamera::ZoomWidget() {
    if (PropertyGrid::Float1("Size", orthographic_size_, nullptr, 1.0f, 10.0f)) {
      RecalculateProjection();
    }
  }
  
  void SceneCamera::RenderGrids(uint32_t max_lines, const glm::mat4& camera_transform, const glm::vec3 camera_pos) {
    if (projection_type_ != ProjectionType::Orthographic or !grid_2d_) {
      return;
    }
    
    static bool init_line_batch = true;
    if (init_line_batch) {
      Batch2DRenderer::AddLineData(max_lines);
      init_line_batch = false;
    }

    float zoom = 0.0f;
    zoom = std::max(GetZoom(), 1.0f);
    
    float hor_line = zoom;
    float ver_line = aspect_ratio_ * zoom;
    float num_lines = std::max(hor_line, ver_line);
    float line_by_2 = num_lines / 2;
    
    if ((hor_line + ver_line) >= max_lines)
      return;
    
    Batch2DRenderer::BeginBatch(projection_matrix_ * glm::inverse(camera_transform));
    
    for (int32_t i = (int32_t)(-line_by_2); i < (int32_t)line_by_2; i++) {
      glm::vec3 car_start_1 = {-line_by_2 + camera_pos.x, 0.5 + i + camera_pos.y, -0.999};
      glm::vec3 car_end_1 = {line_by_2 + camera_pos.x, 0.5 + i + camera_pos.y, -0.999};
      
      glm::vec3 car_start_2 = {0.5 + i + camera_pos.x, -line_by_2 + camera_pos.y, -0.999};
      glm::vec3 car_end_2 = {0.5 + i + camera_pos.x, line_by_2 + camera_pos.y, -0.999};
      
      if (isometric_) {
        glm::vec3 iso_start_1 = glm::vec3(Math::GetIsometricFromCartesian(car_start_1), car_start_1.z);
        glm::vec3 iso_end_1 = glm::vec3(Math::GetIsometricFromCartesian(car_end_1), car_end_1.z);
        glm::vec3 iso_start_2 = glm::vec3(Math::GetIsometricFromCartesian(car_start_2), car_start_2.z);
        glm::vec3 iso_end_2 = glm::vec3(Math::GetIsometricFromCartesian(car_end_2), car_end_2.z);
        
        Batch2DRenderer::DrawLine(iso_start_1, iso_end_1, {0.3, 0.3, 0.3, 1});
        Batch2DRenderer::DrawLine(iso_start_2, iso_end_2, {0.3, 0.3, 0.3, 1});
      }
      else {
        Batch2DRenderer::DrawLine(car_start_1, car_end_1, {0.35, 0.35, 0.35, 1.0});
        Batch2DRenderer::DrawLine(car_start_2, car_end_2, {0.35, 0.35, 0.35, 1.0});
      }
    }
    Batch2DRenderer::EndBatch();
  }
  
} // namespace ikan
