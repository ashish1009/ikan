//
//  open_gl_pipeline.cpp
//  ikan
//
//  Created by Ashish . on 06/04/23.
//

#include "open_gl_pipeline.hpp"
#include "platform/open_gl/open_gl_renderer_id_manager.hpp"
#include <glad/glad.h>

namespace ikan {
  
#define PIPELINE_LOG(...) IK_CORE_DEBUG(LogModule::Pipeline, __VA_ARGS__);
  
  /// This function returns the Open GL Data type from user defined Shader data type
  /// - Parameter type: Shader data type
  static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
    switch (type) {
      case ShaderDataType::NoType:   return 0;
      case ShaderDataType::Float:    return GL_FLOAT;
      case ShaderDataType::Float2:   return GL_FLOAT;
      case ShaderDataType::Float3:   return GL_FLOAT;
      case ShaderDataType::Float4:   return GL_FLOAT;
      case ShaderDataType::Mat3:     return GL_FLOAT;
      case ShaderDataType::Mat4:     return GL_FLOAT;
      case ShaderDataType::Int:      return GL_INT;
      case ShaderDataType::Int2:     return GL_INT;
      case ShaderDataType::Int3:     return GL_INT;
      case ShaderDataType::Int4:     return GL_INT;
      case ShaderDataType::Bool:     return GL_BOOL;
    }
    
    IK_CORE_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
  }
    
  /// This function returns the Open GL Data type from user defined Shader data type
  /// - Parameter type: Shader data type
  static std::string ShaderDataTypeToString(ShaderDataType type) {
    switch (type) {
      case ShaderDataType::NoType:   return "NoType";
      case ShaderDataType::Float:    return "Float ";
      case ShaderDataType::Float2:   return "Float2";
      case ShaderDataType::Float3:   return "Float3";
      case ShaderDataType::Float4:   return "Float4";
      case ShaderDataType::Mat3:     return "Mat3  ";
      case ShaderDataType::Mat4:     return "Mat4  ";
      case ShaderDataType::Int:      return "Int   ";
      case ShaderDataType::Int2:     return "Int2  ";
      case ShaderDataType::Int3:     return "Int3  ";
      case ShaderDataType::Int4:     return "Int4  ";
      case ShaderDataType::Bool:     return "Bool  ";
    }
    
    IK_CORE_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
  }
    
  OpenGLPipeline::OpenGLPipeline() {
    IDManager::GetPipelineId(renderer_id_);
    glBindVertexArray(renderer_id_);
    PIPELINE_LOG("Creating Open GL Pipeline ...");
    PIPELINE_LOG("  Renderer ID {0} ", renderer_id_);
  }
  
  OpenGLPipeline::~OpenGLPipeline() {
    PIPELINE_LOG("Destroying Open GL Pipeline ...");
    PIPELINE_LOG("  Renderer ID {0} ", renderer_id_);
    IDManager::RemovePipelineId(renderer_id_);
  }
  
  void OpenGLPipeline::Bind() const {
    glBindVertexArray(renderer_id_);
    for (auto vb : vertex_buffers_)
      vb->Bind();
    
    if (index_buffer_)
      index_buffer_->Bind();
  }
  
  void OpenGLPipeline::Unbind() const {
    glBindVertexArray(0);
    for (auto vb : vertex_buffers_)
      vb->Unbind();
    
    if (index_buffer_)
      index_buffer_->Unbind();
  }

  void OpenGLPipeline::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertex_buffers) {
    glBindVertexArray(renderer_id_);
    vertex_buffers_.push_back(vertex_buffers);

    uint32_t index = 0;
    const auto& layout = vertex_buffers->GetLayout();
    
    PIPELINE_LOG("  Storing the Vertex Buffer (ID: {0}) into Pipeline (ID: {1}). Total vertrd buffers in pipeline are {2}",
                 vertex_buffers->GetRendererID(), renderer_id_, vertex_buffers_.size());
    Table table("Vertex Attributes (Stride " + std::to_string(layout.GetStride()) + ")" , 4, false);
    table.AddRow({"Name", "Type", "Offset", "Size"});
    
    for (const auto& element : layout.GetElements()) {
      table.AddRow({element.name, ShaderDataTypeToString(element.type), std::to_string(element.offset), std::to_string(element.size)});

      switch (element.type) {
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
        case ShaderDataType::Bool: {
          glEnableVertexAttribArray(index);
          glVertexAttribIPointer(index, (int)element.count, ShaderDataTypeToOpenGLBaseType(element.type),
                                 (int)layout.GetStride(), (const void*)element.offset);
          index++;
          break;
        }
          
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4: {
          glEnableVertexAttribArray(index);
          glVertexAttribPointer(index, (int)element.count, ShaderDataTypeToOpenGLBaseType(element.type),
                                element.normalized ? GL_TRUE : GL_FALSE, (int)layout.GetStride(), (const void*)element.offset);
          index++;
          break;
        }
        case ShaderDataType::Mat3:
        case ShaderDataType::Mat4: {
          uint32_t count = element.count;
          for (uint8_t i = 0; i < count; i++) {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, (int)count, ShaderDataTypeToOpenGLBaseType(element.type),
                                  element.normalized ? GL_TRUE : GL_FALSE, (int)layout.GetStride(), (const void*)(sizeof(float) * count * i));
            glVertexAttribDivisor(index, 1);
            index++;
          }
          break;
        }
        default: {
          IK_CORE_ASSERT(false, "Unknown ShaderDataType!");
        }
      } // switch (element.Type)
    } // for (const auto& element : layout.GetElements())
    
    table.Dump(Logger::Level::Trace, "Pipeline");
  }

  void OpenGLPipeline::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& index_buffer) {
    PIPELINE_LOG("  Setting the Index Buffer (ID: {0}) into Pipeline (ID: {1})", index_buffer->GetRendererID(), renderer_id_);
    index_buffer_ = index_buffer;
  }

} // namespace ikan
