//
//  open_gl_shader_uniforms.cpp
//  ikan
//
//  Created by Ashish . on 06/04/23.
//

#include "open_gl_shader_uniforms.hpp"

namespace ikan {
  
  // --------------------------------------------------------------------------
  // OpenGLShaderUniformDeclaration
  // --------------------------------------------------------------------------
  uint32_t OpenGLShaderUniformDeclaration::SizeOfUniformType(Type type) {
    switch (type) {
      case OpenGLShaderUniformDeclaration::Type::None:
        return 0;
      case OpenGLShaderUniformDeclaration::Type::Bool:
      case OpenGLShaderUniformDeclaration::Type::Int32:
      case OpenGLShaderUniformDeclaration::Type::Float32:
        return 4;
      case OpenGLShaderUniformDeclaration::Type::Vec2:
        return 4 * 2;
      case OpenGLShaderUniformDeclaration::Type::Vec3:
        return 4 * 3;
      case OpenGLShaderUniformDeclaration::Type::Vec4:
        return 4 * 4;
      case OpenGLShaderUniformDeclaration::Type::Mat3:
        return 4 * 3 * 3;
      case OpenGLShaderUniformDeclaration::Type::Mat4:
        return 4 * 4 * 4;
      case OpenGLShaderUniformDeclaration::Type::Struct:
        IK_CORE_ASSERT(false, "No need to return the size in case of struct")
    }
    return 0;
  }
  
  OpenGLShaderUniformDeclaration::Type OpenGLShaderUniformDeclaration::StringToType(const std::string& type) {
    if (type == "bool")     return Type::Bool;
    if (type == "int")      return Type::Int32;
    if (type == "float")    return Type::Float32;
    if (type == "vec2")     return Type::Vec2;
    if (type == "vec3")     return Type::Vec3;
    if (type == "vec4")     return Type::Vec4;
    if (type == "mat3")     return Type::Mat3;
    if (type == "mat4")     return Type::Mat4;
    
    return Type::None;
  }
  
  std::string OpenGLShaderUniformDeclaration::TypeToString(Type type) {
    switch (type) {
      case OpenGLShaderUniformDeclaration::Type::None:       return "None  ";
      case OpenGLShaderUniformDeclaration::Type::Bool:       return "bool  ";
      case OpenGLShaderUniformDeclaration::Type::Int32:      return "int32 ";
      case OpenGLShaderUniformDeclaration::Type::Float32:    return "float ";
      case OpenGLShaderUniformDeclaration::Type::Vec2:       return "vec2  ";
      case OpenGLShaderUniformDeclaration::Type::Vec3:       return "vec3  ";
      case OpenGLShaderUniformDeclaration::Type::Vec4:       return "vec4  ";
      case OpenGLShaderUniformDeclaration::Type::Mat3:       return "mat3  ";
      case OpenGLShaderUniformDeclaration::Type::Mat4:       return "mat4  ";
      case OpenGLShaderUniformDeclaration::Type::Struct:     return "struct";
    }
    return "Invalid Type";
  }
  
  OpenGLShaderUniformDeclaration::OpenGLShaderUniformDeclaration(ShaderDomain domain, Type type, const std::string& name, uint32_t count)
  : struct_(nullptr), type_(type), domain_(domain), name_(name), count_(count) {
    size_ = SizeOfUniformType(type_) * count_;
    IK_CORE_DEBUG(LogModule::Shader, "      {0} {1}[{2}] (Size : {3}) ", TypeToString(type_), name_, count_, size_);
  }
  
  OpenGLShaderUniformDeclaration::OpenGLShaderUniformDeclaration(ShaderDomain domain, ShaderStruct* uniform_struct,
                                                                 const std::string& name, uint32_t count)
  : struct_(uniform_struct), type_(OpenGLShaderUniformDeclaration::Type::Struct),
  domain_(domain), name_(name), count_(count) {
    size_ = struct_->GetSize() * count_;
    IK_CORE_DEBUG(LogModule::Shader, "      {0} {1}[{2}] (Size : {3}) ", TypeToString(type_), name_, count_, size_);
  }
  
  OpenGLShaderUniformDeclaration::~OpenGLShaderUniformDeclaration() {
    IK_CORE_DEBUG(LogModule::Shader, "      Destroying : {0} {1}[{2}] (Size : {3}) ", TypeToString(type_), name_, count_, size_);
  }
  
  void OpenGLShaderUniformDeclaration::SetOffset(uint32_t offset) {
    if (type_ == OpenGLShaderUniformDeclaration::Type::Struct)
      struct_->SetOffset(offset);
    offset_ = offset;
  }
  
  // --------------------------------------------------------------------------
  // OpenGLShaderResourceDeclaration
  // --------------------------------------------------------------------------
  OpenGLShaderResourceDeclaration::Type
  OpenGLShaderResourceDeclaration::StringToType(const std::string& type) {
    if (type == "sampler2D")    return Type::kTexture2D;
    if (type == "sampler2DMS")  return Type::kTexture2D;
    if (type == "samplerCube")  return Type::kTextureCubeMap;
    
    return Type::kNone;
  }
  
  std::string OpenGLShaderResourceDeclaration::TypeToString(Type type) {
    switch (type) {
      case Type::kNone:           return "None       ";
      case Type::kTexture2D:      return "sampler2D  ";
      case Type::kTextureCubeMap: return "samplerCube";
    }
    return "Invalid Type";
  }
  
  OpenGLShaderResourceDeclaration::OpenGLShaderResourceDeclaration(Type type, const std::string& name, uint32_t count)
  : type_(type), name_(name), count_(count) {
    IK_CORE_DEBUG(LogModule::Shader, "      {0} {1}[{2}]", TypeToString(type_), name_, count_);
  }
  
  OpenGLShaderResourceDeclaration::~OpenGLShaderResourceDeclaration() {
    IK_CORE_DEBUG(LogModule::Shader, "      Destroying : {0} {1}[{2}]", TypeToString(type_), name_, count_);
  }
  
  // --------------------------------------------------------------------------
  // OpenGLShaderUniformBufferDeclaration
  // --------------------------------------------------------------------------
  OpenGLShaderUniformBufferDeclaration::OpenGLShaderUniformBufferDeclaration(const std::string& name, ShaderDomain domain)
  : name_(name), domain_(domain), size_(0), register_(0) {
  }
  
  OpenGLShaderUniformBufferDeclaration::~OpenGLShaderUniformBufferDeclaration() {
    for (auto* uniform : uniforms_) {
      delete uniform;
      uniform = nullptr;
    }
    
    uniforms_.clear();
  }
  
  void OpenGLShaderUniformBufferDeclaration::PushUniform(OpenGLShaderUniformDeclaration* uniform) {
    uint32_t offset = 0;
    if (uniforms_.size()) {
      OpenGLShaderUniformDeclaration*
      previous = (OpenGLShaderUniformDeclaration*)uniforms_.back();
      offset = previous->offset_ + previous->size_;
    }
    uniform->SetOffset(offset);
    size_ += uniform->GetSize();
    uniforms_.push_back(uniform);
  }
  
  ShaderUniformDeclaration* OpenGLShaderUniformBufferDeclaration::FindUniform(const std::string& name) {
    for (ShaderUniformDeclaration* uniform : uniforms_) {
      if (uniform->GetName() == name)
        return uniform;
    }
    return nullptr;
  }
    
} // namespace ikan
