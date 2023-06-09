//
//  open_gl_texture.cpp
//  ikan
//
//  Created by Ashish . on 06/04/23.
//

#include "open_gl_texture.hpp"
#include "platform/open_gl/open_gl_renderer_id_manager.hpp"
#include "renderer/utils/renderer_stats.hpp"

#include <stb_image.h>

namespace ikan {
  
  namespace texture_utils {
    
#ifdef IK_DEBUG_FEATURE
    
    std::string GetFormatNameFromEnum(uint32_t format) {
      if (format == GL_RGBA8)       return "GL_RGBA8";
      if (format == GL_RGBA)        return "GL_RGBA";
      if (format == GL_RGB8)        return "GL_RGB8";
      if (format == GL_RGB)         return "GL_RGB";
      if (format == GL_RED)         return "GL_RED";
      if (format == GL_R32I)        return "GL_R32I";
      if (format == GL_RED_INTEGER) return "GL_RED_INTEGER";
      
      if (format == GL_DEPTH_COMPONENT)   return "GL_DEPTH_COMPONENT";
      else IK_CORE_ASSERT(false, "Add New Featured format herer too");
    }
    
#endif
    
    GLint ikanFormatToOpenGLFormat(TextureFormat format) {
      switch (format) {
        case TextureFormat::None:    return (GLint)0;
        case TextureFormat::RGBA:    return GL_RGBA;
      }
      return (GLint)0;
    }
    
    GLint GetTextureType(GLint format_type) {
      switch (format_type) {
        case GL_RGBA8:
        case GL_RGB8:
        case GL_RGBA:
        case GL_RED:
        case GL_R32I:
          return GL_UNSIGNED_BYTE;
          
        case GL_DEPTH_COMPONENT: return GL_FLOAT;
          
        default:
          IK_CORE_ASSERT(false, "Add other formats");
      }
    }
    
  } // namespace texture_utils
  
  OpenGLTexture::OpenGLTexture(uint32_t width, uint32_t height, void* data, uint32_t size)
  : width_((int32_t)width), height_((int32_t)height),
  internal_format_(GL_RGBA8), data_format_(GL_RGBA), size_(size) {
    // Create the buffer to store the white texture
    texture_data_ = new uint32_t;
    memcpy(texture_data_, data, size_);
    
    IDManager::GetTextureId(&renderer_id_);
    glBindTexture(GL_TEXTURE_2D, renderer_id_);
    
    // Setup Texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    uint16_t bpp = data_format_ == GL_RGBA ? 4 : 3;
    IK_CORE_ASSERT((size_ == (uint32_t)width_ * (uint32_t)height_ * bpp), "Data must be entire texture");
    
    // Create texture in the renderer Buffer
    glTexImage2D(GL_TEXTURE_2D, 0, /* level */ (GLint)internal_format_, (GLsizei)width_, (GLsizei)height_,
                 0, /* Border */ data_format_, texture_utils::GetTextureType(internal_format_), (stbi_uc*)(texture_data_));
    
    // Set the flag if uploaded
    uploaded_ = true;
    
    // Increment the size in stats
    RendererStatistics::Get().texture_buffer_size += size_;
    
    IK_CORE_DEBUG(LogModule::Texture, "Creating Open GL White Texture ... ");
    IK_CORE_DEBUG(LogModule::Texture, "  Renderer ID        {0}  ", renderer_id_);
    IK_CORE_DEBUG(LogModule::Texture, "  Width              {0}  ", width_);
    IK_CORE_DEBUG(LogModule::Texture, "  Height             {0}  ", height_);
    IK_CORE_DEBUG(LogModule::Texture, "  Size               {0} B", size_);
    IK_CORE_DEBUG(LogModule::Texture, "  Number of Channel  {0}  ", channel_);
    IK_CORE_DEBUG(LogModule::Texture, "  InternalFormat     {0}  ", texture_utils::GetFormatNameFromEnum(internal_format_));
    IK_CORE_DEBUG(LogModule::Texture, "  DataFormat         {0}  ", texture_utils::GetFormatNameFromEnum(data_format_));
    
    delete (uint32_t*)texture_data_;
  }
  
  OpenGLTexture::OpenGLTexture(const std::string& file_path, bool linear)
  : file_path_(file_path), name_(StringUtils::GetNameFromFilePath(file_path)),
  internal_format_(GL_RGBA8), data_format_(GL_RGBA) {
    if (renderer_id_)
      IDManager::RemoveTextureId(&renderer_id_);

    // Invert the texture. as by default open gl load inverted vertically
    stbi_set_flip_vertically_on_load(1);
    
    // Load the file with stb image API
    texture_data_ = stbi_load(file_path_.c_str(), &width_, &height_, &channel_, 0 /* desired_channels */ );
    
    // If file loaded successfullt
    if (texture_data_) {
      uploaded_ = true;
      switch (channel_) {
        case 4 :
          internal_format_ = GL_RGBA8;
          data_format_     = GL_RGBA;
          break;
        case 3 :
          internal_format_ = GL_RGB8;
          data_format_     = GL_RGB;
          break;
        case 2 :
        case 1 :
          internal_format_ = GL_RED;
          data_format_     = GL_RED;
          break;
          
        default:
          IK_CORE_ASSERT(false, "Invalid Format ");
      }
      
      IDManager::GetTextureId(&renderer_id_);
      glBindTexture(GL_TEXTURE_2D, renderer_id_);
      
      // Setup min and Mag filter
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (linear ? GL_LINEAR : GL_NEAREST));
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (linear ? GL_LINEAR : GL_NEAREST));
      
      // Texuter Flags
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      
      // Create texture in the renderer Buffer
      glTexImage2D(GL_TEXTURE_2D, 0, /* Level */ (GLint)internal_format_, width_, height_,
                   0, /* Border */ data_format_, texture_utils::GetTextureType(internal_format_), texture_data_);
      
      size_ = (uint32_t)width_ * (uint32_t)height_ * (uint32_t)channel_;
      
      // Increment the size in stats
      RendererStatistics::Get().texture_buffer_size += size_;
      
      // Delete the data as we have already loaded in graphics
      delete (stbi_uc*)texture_data_;
    }
    
    if (uploaded_) {
      IK_CORE_DEBUG(LogModule::Texture, "Creating Open GL Texture from File ... ");
      IK_CORE_DEBUG(LogModule::Texture, "  File Path          {0}", file_path_);
      IK_CORE_DEBUG(LogModule::Texture, "  Renderer ID        {0}", renderer_id_);
      IK_CORE_DEBUG(LogModule::Texture, "  Width              {0}", width_);
      IK_CORE_DEBUG(LogModule::Texture, "  Height             {0}", height_);
      IK_CORE_DEBUG(LogModule::Texture, "  Size               {0} B", size_);
      IK_CORE_DEBUG(LogModule::Texture, "  Number of Channel  {0}", channel_);
      IK_CORE_DEBUG(LogModule::Texture, "  InternalFormat     {0}", texture_utils::GetFormatNameFromEnum(internal_format_));
      IK_CORE_DEBUG(LogModule::Texture, "  DataFormat         {0}", texture_utils::GetFormatNameFromEnum(data_format_));
    } else {
#ifdef IK_DEBUG_FEATURE
      IK_CORE_ASSERT(false, "Failed to load stbi Image. Check the path from Logs");
#endif
    }
  }
  
  OpenGLTexture::~OpenGLTexture() {
    if (uploaded_) {
      IK_CORE_DEBUG(LogModule::Texture, "Destroying Open GL Texture: !!! ");
      if (file_path_ != "")
        IK_CORE_DEBUG(LogModule::Texture, "  File Path          {0}", file_path_);
      
      IK_CORE_DEBUG(LogModule::Texture, "  Renderer ID        {0}", renderer_id_);
      IK_CORE_DEBUG(LogModule::Texture, "  Width              {0}", width_);
      IK_CORE_DEBUG(LogModule::Texture, "  Height             {0}", height_);
      IK_CORE_DEBUG(LogModule::Texture, "  Size               {0} B", size_);
      IK_CORE_DEBUG(LogModule::Texture, "  Number of Channel  {0}", channel_);
      IK_CORE_DEBUG(LogModule::Texture, "  InternalFormat     {0}", texture_utils::GetFormatNameFromEnum(internal_format_));
      IK_CORE_DEBUG(LogModule::Texture, "  DataFormat         {0}", texture_utils::GetFormatNameFromEnum(data_format_));
      
      IDManager::RemoveTextureId(&renderer_id_);
      RendererStatistics::Get().texture_buffer_size -= size_;
    }
  }
  
  void OpenGLTexture::Bind(uint32_t slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, renderer_id_);
  }
  
  void OpenGLTexture::Unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  
  OpenGLCharTexture::OpenGLCharTexture(const FT_Face& face, const glm::ivec2& size, const glm::ivec2& bearing, uint32_t advance,
                                       [[maybe_unused]] char char_val)
  : size_(size), bearing_(bearing), advance_(advance) {
    // Generate the renderer IF if not exist in map already
    IDManager::GetTextureId(&renderer_id_);
    
    glBindTexture(GL_TEXTURE_2D, renderer_id_);
    
    // Create texture in the renderer Buffer
    glTexImage2D(GL_TEXTURE_2D, 0, /* Level */ GL_RED, (GLsizei)face->glyph->bitmap.width, (GLsizei)face->glyph->bitmap.rows,
                 0, /* Border */ GL_RED, texture_utils::GetTextureType(GL_RED), face->glyph->bitmap.buffer );
    
    width_ = (uint32_t)face->glyph->bitmap.width;
    height_ = (uint32_t)face->glyph->bitmap.rows;
    
    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    IK_CORE_TRACE(LogModule::CharTexture, "Creating Open GL Char Texture to store Char {0} ( Renderer ID {1} )", char_val, renderer_id_);
    
    // Increment the size in stats
    data_size_ = face->glyph->bitmap.width * face->glyph->bitmap.rows;
    RendererStatistics::Get().texture_buffer_size += data_size_;
  }
  
  /// Open GL Texture Destructor
  OpenGLCharTexture::~OpenGLCharTexture() {
    IDManager::RemoveTextureId(&renderer_id_);
    RendererStatistics::Get().texture_buffer_size -= data_size_;
  }
  
  void OpenGLCharTexture::Bind(uint32_t slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, renderer_id_);
  }
  void OpenGLCharTexture::Unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
  }

} // namespace ikan
