//
//  shader_uniforms.hpp
//  ikan
//
//  Created by Ashish . on 06/04/23.
//

#pragma once

namespace ikan {
  
  /// This enum stores the type of shader
  enum class ShaderDomain {
    None = 0,
    Vertex = 1,
    Fragment = 2,
    Geometry = 3
  };
  
  /// This class stores shader uniform elements (like int, float, vec ....)
  class ShaderUniformDeclaration {
  private:
    friend class Shader;
    friend class OpenGLShader;
    friend class ShaderStruct;
    
  public:
    virtual ~ShaderUniformDeclaration() = default;
    
    /// This function returns the name of field
    virtual const std::string& GetName() const = 0;
    /// This functionr returns the size of field
    virtual uint32_t GetSize() const = 0;
    /// This function returns the count of field
    virtual uint32_t GetCount() const = 0;
    /// This function returns the offset of field
    virtual uint32_t GetOffset() const = 0;
    
    /// This function returns the domain of shader
    virtual ShaderDomain GetDomain() const = 0;
    
  protected:
    /// This function update the offset of the field
    virtual void SetOffset(uint32_t offset) = 0;
  };
  
  class ShaderUniformBufferDeclaration {
  public:
    /// This is the defualt destructor
    virtual ~ShaderUniformBufferDeclaration() = default;
    
    /// This function returns the name of buffer
    virtual const std::string& GetName() const = 0;
    /// This function returns the register of buffer
    virtual uint32_t GetRegister() const = 0;
    /// This function returns the size of buffer
    virtual uint32_t GetSize() const = 0;
    /// This function returns all the declaration in buffer
    virtual const std::vector<ShaderUniformDeclaration*>& GetUniformDeclarations() const = 0;
    /// This function finds the uniform declaretion by name
    /// - Parameter name: name of uniform
    virtual ShaderUniformDeclaration* FindUniform(const std::string& name) = 0;
  };
  
  /// This class stores the uniform resource like sampler 2D ...
  class ShaderResourceDeclaration {
  public:
    /// This is the default destructor
    virtual ~ShaderResourceDeclaration() = default;
    
    /// This function returns the name of resrouce
    virtual const std::string& GetName() const = 0;
    /// This function returns the register ot resource
    virtual uint32_t GetRegister() const = 0;
    /// This function returns the count of array of resource
    virtual uint32_t GetCount() const = 0;
  };
  
  /// This class stores the content of a structure
  class ShaderStruct {
  public:
    /// This constructor creates the structure instance for a shader
    /// - Parameter name: name of structure
    ShaderStruct(const std::string& name) : name_(name), size_(0), offset_(0) {}
    /// Destroy the structure
    ~ShaderStruct() {
      for (auto& field : fields_)
        delete field;
    }
    
    /// This function updates the offset value of structure
    /// - Parameter offset: new offset
    void SetOffset(uint32_t offset)  { offset_ = offset; }
    
    /// This function returns the size of structure in bytes
    uint32_t GetSize() const { return size_; }
    /// This function returns the offset of structure
    uint32_t GetOffset() const { return offset_; }
    /// This function returns the name of structure
    const std::string& GetName() const { return name_; }
    /// This function returns the all the fields of structure
    const std::vector<ShaderUniformDeclaration*>& GetFields() const { return fields_; }
    
    /// This function adds a new field in structure
    void AddField(ShaderUniformDeclaration* field) {
      size_ += field->GetSize();
      uint32_t offset = 0;
      if (fields_.size()) {
        ShaderUniformDeclaration* previous = fields_.back();
        offset = previous->GetOffset() + previous->GetSize();
      }
      field->SetOffset(offset);
      fields_.push_back(field);
    }
    
    DELETE_COPY_MOVE_CONSTRUCTORS(ShaderStruct)
    
  private:
    std::string name_;
    
    uint32_t size_ = 0;
    uint32_t offset_= 0;
    
    // Stores each field of structure
    std::vector<ShaderUniformDeclaration*> fields_;
    
    friend class OpenGLShader;
    friend class Shader;
  };
  
} // namespace ikan
