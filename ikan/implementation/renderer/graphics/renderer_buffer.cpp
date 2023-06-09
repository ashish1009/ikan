//
//  renderer_buffer.cpp
//  ikan
//
//  Created by Ashish . on 06/04/23.
//

#include "renderer_buffer.hpp"
#include "platform/open_gl/open_gl_renderer_buffer.hpp"

namespace ikan {
  
  std::shared_ptr<VertexBuffer> VertexBuffer::Create(void *data, uint32_t size) {
    switch (Renderer::GetApi()) {
      case Renderer::Api::OpenGl: return std::make_shared<OpenGLVertexBuffer>(data, size);
      case Renderer::Api::None:
      default:
        IK_CORE_ASSERT(false, "Invalid Renderer API (None)"); break;
    }
  }
  
  std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size) {
    switch (Renderer::GetApi()) {
      case Renderer::Api::OpenGl: return std::make_shared<OpenGLVertexBuffer>(size);
      case Renderer::Api::None:
      default:
        IK_CORE_ASSERT(false, "Invalid Renderer API (None)"); break;
    }
  }
  
  std::shared_ptr<IndexBuffer> IndexBuffer::CreateWithCount(void* data, uint32_t count) {
    switch (Renderer::GetApi()) {
      case Renderer::Api::OpenGl: return std::make_shared<OpenGLIndexBuffer>(data, count * sizeof(uint32_t));
      case Renderer::Api::None:
      default:
        IK_CORE_ASSERT(false, "Invalid Renderer API (None)"); break;
    }
  }
  std::shared_ptr<IndexBuffer> IndexBuffer::CreateWithSize(void* data, uint32_t size) {
    switch (Renderer::GetApi()) {
      case Renderer::Api::OpenGl: return std::make_shared<OpenGLIndexBuffer>(data, size);
      case Renderer::Api::None:
      default:
        IK_CORE_ASSERT(false, "Invalid Renderer API (None)"); break;
    }
  }

  /// This function returns the number of bytes in Shader data types. (Size)
  /// - Parameter type: Shader data type
  static uint32_t ShaderDataTypeSize(ShaderDataType type) {
    switch (type) {
      case ShaderDataType::NoType : return 0;
      case ShaderDataType::Float  : return 4;
      case ShaderDataType::Float2 : return 4 * 2;
      case ShaderDataType::Float3 : return 4 * 3;
      case ShaderDataType::Float4 : return 4 * 4;
      case ShaderDataType::Mat3   : return 4 * 3 * 3;
      case ShaderDataType::Mat4   : return 4 * 4 * 5;
      case ShaderDataType::Int    : return 4;
      case ShaderDataType::Int2   : return 4 * 2;
      case ShaderDataType::Int3   : return 4 * 3;
      case ShaderDataType::Int4   : return 4 * 4;
      case ShaderDataType::Bool   : return 1;
    }
    IK_CORE_ASSERT(false, "Invalid Type");
    return 0;
  }
  
  /// This function returns the number of same data type elements. (Count)
  /// - Parameter type: Shader data type
  static uint32_t GetElementCount(ShaderDataType type) {
    switch (type) {
      case ShaderDataType::NoType : return 0;
      case ShaderDataType::Float  : return 1;
      case ShaderDataType::Float2 : return 2;
      case ShaderDataType::Float3 : return 3;
      case ShaderDataType::Float4 : return 4;
      case ShaderDataType::Mat3   : return 3;
      case ShaderDataType::Mat4   : return 4;
      case ShaderDataType::Int    : return 1;
      case ShaderDataType::Int2   : return 2;
      case ShaderDataType::Int3   : return 3;
      case ShaderDataType::Int4   : return 4;
      case ShaderDataType::Bool   : return 1;
    }
    IK_CORE_ASSERT(false, "Unkown Shader datatype!! ");
  }
  
  BufferElement::BufferElement(const std::string& name, ShaderDataType type, bool normalized)
  : name(name), type(type), size(ShaderDataTypeSize(type)), count(GetElementCount(type)), offset(0), normalized(normalized) { }

  BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
  : elements_(elements) {
    CalculateOffsetAndStride();
  }
  
  void BufferLayout::CalculateOffsetAndStride() {
    size_t offset = 0;
    stride_ = 0;
    for (auto& element : elements_) {
      element.offset = offset;
      offset += element.size;
      stride_ += element.size;
    }
  }
  
  const std::vector<BufferElement> BufferLayout::GetElements() const { return elements_; }
  uint32_t BufferLayout::GetStride() const { return stride_; }
  std::vector<BufferElement>::iterator BufferLayout::begin() { return elements_.begin(); }
  std::vector<BufferElement>::iterator BufferLayout::end() { return elements_.end(); }
  std::vector<BufferElement>::const_iterator BufferLayout::begin() const { return elements_.begin(); }
  std::vector<BufferElement>::const_iterator BufferLayout::end() const { return elements_.end(); }
  
  BufferLayout::BufferLayout(const BufferLayout& other)
  : stride_(other.stride_) {
    for (const auto& elem : other.elements_)
      elements_.emplace_back(elem);
  }
  BufferLayout& BufferLayout::operator=(const BufferLayout& other) {
    stride_ = other.stride_;
    elements_.clear();
    for (const auto& elem : other.elements_)
      elements_.emplace_back(elem);
    return *this;
  }
  
} // namespace ikan
