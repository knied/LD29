//
//  GLClasses.h
//  ComponentBasedGame
//
//  Created by Kristof Niederholtmeyer on 02.10.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __ComponentBasedGame__GLClasses__
#define __ComponentBasedGame__GLClasses__

////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <OpenGL/gl3.h>

#include "Math.h"
//#include "Image.h"

////////////////////////////////////////////////////////////////////////////////
// Some internaly used helpers

namespace GLInternal {
    // OpenGL type system
    template <typename Type>
    struct GLType {};
    
    template<>
    struct GLType<float> {
        enum : GLint {size = 1}; // elements of a vector type
        enum : GLenum {type = GL_FLOAT}; // opengl enum type
    };
    
    template<unsigned int N>
    struct GLType<Vector<float, N>> {
        enum : GLint {size = N}; // elements of a vector type
        enum : GLenum {type = GL_FLOAT}; // opengl enum type
    };
    
    template<>
    struct GLType<int> {
        enum : GLint {size = 1}; // elements of a vector type
        enum : GLenum {type = GL_INT}; // opengl enum type
    };
    
    template<unsigned int N>
    struct GLType<Vector<int, N>> {
        enum : GLint {size = N}; // elements of a vector type
        enum : GLenum {type = GL_INT}; // opengl enum type
    };
    
    template<>
    struct GLType<unsigned int> {
        enum : GLint {size = 1}; // elements of a vector type
        enum : GLenum {type = GL_UNSIGNED_INT}; // opengl enum type
    };
    
    template<unsigned int N>
    struct GLType<Vector<unsigned int, N>> {
        enum : GLint {size = N}; // elements of a vector type
        enum : GLenum {type = GL_UNSIGNED_INT}; // opengl enum type
    };
    
    template<>
    struct GLType<short> {
        enum : GLint {size = 1}; // elements of a vector type
        enum : GLenum {type = GL_SHORT}; // opengl enum type
    };
    
    template<unsigned int N>
    struct GLType<Vector<short, N>> {
        enum : GLint {size = N}; // elements of a vector type
        enum : GLenum {type = GL_SHORT}; // opengl enum type
    };
    
    template<>
    struct GLType<unsigned short> {
        enum : GLint {size = 1}; // elements of a vector type
        enum : GLenum {type = GL_UNSIGNED_SHORT}; // opengl enum type
    };
    
    template<unsigned int N>
    struct GLType<Vector<unsigned short, N>> {
        enum : GLint {size = N}; // elements of a vector type
        enum : GLenum {type = GL_UNSIGNED_SHORT}; // opengl enum type
    };
    
    template<>
    struct GLType<char> {
        enum : GLint {size = 1}; // elements of a vector type
        enum : GLenum {type = GL_BYTE}; // opengl enum type
    };
    
    template<unsigned int N>
    struct GLType<Vector<char, N>> {
        enum : GLint {size = N}; // elements of a vector type
        enum : GLenum {type = GL_BYTE}; // opengl enum type
    };
    
    template<>
    struct GLType<unsigned char> {
        enum : GLint {size = 1}; // elements of a vector type
        enum : GLenum {type = GL_UNSIGNED_BYTE}; // opengl enum type
    };
    
    template<unsigned int N>
    struct GLType<Vector<unsigned char, N>> {
        enum : GLint {size = N}; // elements of a vector type
        enum : GLenum {type = GL_UNSIGNED_BYTE}; // opengl enum type
    };
    
    template<typename Type>
    struct GLColorType {
        enum : GLenum {type = GLType<Type>::type};
        enum : GLenum {format = GL_RED};
    };
    
    template<typename Type>
    struct GLColorType<Vector<Type, 1>> {
        enum : GLenum {type = GLType<Type>::type};
        enum : GLenum {format = GL_RED};
    };
    
    template<typename Type>
    struct GLColorType<Vector<Type, 2>> {
        enum : GLenum {type = GLType<Type>::type};
        enum : GLenum {format = GL_RG};
    };
    
    template<typename Type>
    struct GLColorType<Vector<Type, 3>> {
        enum : GLenum {type = GLType<Type>::type};
        enum : GLenum {format = GL_RGB};
    };
    
    template<typename Type>
    struct GLColorType<Vector<Type, 4>> {
        enum : GLenum {type = GLType<Type>::type};
        enum : GLenum {format = GL_RGBA};
    };
    
    template<typename Type>
    struct GLBytePacker {
        enum { size = sizeof(Type) };
        static void pack(uint8_t* dst, Type const& src) {
            *(Type*)dst = src;
        }
    };
    
    template<unsigned int N, typename Type>
    struct GLBytePacker<Vector<Type, N>> {
        enum { size = N * GLBytePacker<Type>::size };
        static void pack(uint8_t* dst, Vector<Type, N> const& src) {
            for (int i = 0; i < N; ++i) {
                GLBytePacker<Type>::pack(dst + i * GLBytePacker<Type>::size, src[i]);
            }
        }
    };
    
    template<typename Type, typename ...Types>
    struct GLListBytePacker {
        enum { size = GLBytePacker<Type>::size + GLListBytePacker<Types...>::size };
        static void pack(uint8_t* dst, Type const& src, Types const&... srcs) {
            GLBytePacker<Type>::pack(dst, src);
            GLListBytePacker<Types...>::pack(dst + GLBytePacker<Type>::size, srcs...);
        }
    };
    
    template<typename Type>
    struct GLListBytePacker<Type> {
        enum { size = GLBytePacker<Type>::size };
        static void pack(uint8_t* dst, Type const& src) {
            GLBytePacker<Type>::pack(dst, src);
        }
    };
    
    template<int Number, int ...Numbers>
    struct GLSum {
        enum { value = Number + GLSum<Numbers...>::value };
    };
    
    template<int Number>
    struct GLSum<Number> {
        enum { value = Number };
    };
    
    // stores runtime information for an OpenGL attribute
    struct GLAttributeInfo {
        GLint size; // number of elements in a vector type
        GLenum type; // type identifier
        GLsizei stride; // size in byte of a whole vertex
        GLsizei offset; // offset in byte into a vertex
    };
    
    // determine runtime type information for OpenGL
    template<unsigned int Index, typename Type, typename ...Types>
    struct GLAttributeInfoHelper {
        enum : GLint {size = GLAttributeInfoHelper<Index-1, Types...>::size};
        enum : GLenum {type = GLAttributeInfoHelper<Index-1, Types...>::type};
        enum : GLsizei {offset = GLInternal::GLBytePacker<Type>::size + GLAttributeInfoHelper<Index-1, Types...>::offset};
    };
    // end of recurrsion
    template<typename Type, typename ...Types>
    struct GLAttributeInfoHelper<0, Type, Types...> {
        enum : GLint {size = GLType<Type>::size};
        enum : GLenum {type = GLType<Type>::type};
        enum : GLsizei {offset = 0};
    };
    
    // determine the attribute information for a list of types
    template<unsigned int Index, typename ...Types>
    struct GLGetAttributeInfoHelper {
        static void get(std::vector<GLAttributeInfo>& attribute_info) {
            GLGetAttributeInfoHelper<Index-1, Types...>::get(attribute_info);
            enum : GLsizei {stride = GLInternal::GLSum<GLInternal::GLBytePacker<Types>::size...>::value};
            attribute_info[Index].size = GLAttributeInfoHelper<Index, Types...>::size;
            attribute_info[Index].type = GLAttributeInfoHelper<Index, Types...>::type;
            attribute_info[Index].stride = stride;
            attribute_info[Index].offset = GLAttributeInfoHelper<Index, Types...>::offset;
        }
    };
    // end of recurrsion
    template<typename ...Types>
    struct GLGetAttributeInfoHelper<0, Types...> {
        static void get(std::vector<GLAttributeInfo>& attribute_info) {
            enum : GLsizei {stride = GLInternal::GLSum<GLInternal::GLBytePacker<Types>::size...>::value};
            attribute_info[0].size = GLAttributeInfoHelper<0, Types...>::size;
            attribute_info[0].type = GLAttributeInfoHelper<0, Types...>::type;
            attribute_info[0].stride = stride;
            attribute_info[0].offset = GLAttributeInfoHelper<0, Types...>::offset;
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
// The OpenGL buffer resource

class GLBindBuffer;

class GLBuffer {
    GLuint _buffer;
    std::vector<GLInternal::GLAttributeInfo> _attribute_info;
    GLsizei _size;
    
    GLBuffer(const GLBuffer&);
    const GLBuffer& operator = (const GLBuffer&);
    
public:
    friend class GLBindBuffer;
    
    GLBuffer() : _buffer(0) {
        glGenBuffers(1, &_buffer);
    }
    GLBuffer(GLBuffer&& mesh) : _buffer(std::move(mesh._buffer)), _attribute_info(std::move(mesh._attribute_info)), _size(std::move(mesh._size)) {
        mesh._buffer = 0;
    }
    ~GLBuffer() {
        if (_buffer != 0) {
            glDeleteBuffers(1, &_buffer);
        }
    }
    const GLBuffer& operator = (GLBuffer&& mesh) {
        if (_buffer != 0) {
            glDeleteBuffers(1, &_buffer);
        }
        
        _buffer = std::move(mesh._buffer);
        _attribute_info = std::move(mesh._attribute_info);
        _size = std::move(mesh._size);
        mesh._buffer = 0;
        return *this;
    }
    
    size_t attribute_count() const {
        return _attribute_info.size();
    }
    GLint attribute_size(size_t attribute) const {
        return _attribute_info[attribute].size;
    }
    GLenum attribute_type(size_t attribute) const {
        return _attribute_info[attribute].type;
    }
    GLsizei attribute_stride(size_t attribute) const {
        return _attribute_info[attribute].stride;
    }
    GLsizei attribute_offset(size_t attribute) const {
        return _attribute_info[attribute].offset;
    }
    GLsizei size() const {
        return _size;
    }
    GLuint buffer() const {
        return _buffer;
    }
}; // GLBuffer

namespace GLInternal {
    struct GLBindBufferLock {
        static GLuint _bound_buffer;
        static GLuint _bound_vertex_array;
    }; // GLBindBufferLock
}

////////////////////////////////////////////////////////////////////////////////
// A helper to bind and manipulate the buffer resource. Only one buffer may be
// bound at a time.

class GLBindBuffer : private GLInternal::GLBindBufferLock {
    // no copy
    GLBindBuffer(const GLBindBuffer&);
    const GLBindBuffer& operator = (const GLBindBuffer&);
    
    GLBuffer& _buffer;
    
public:
    GLBindBuffer(GLBuffer& buffer) : _buffer(buffer) {
        if (_bound_vertex_array != 0) {
            throw std::runtime_error("GLClasses: Trying to bind a GLBuffer while a GLVertexArray is bound.");
        }
        if (_bound_buffer != 0) {
            throw std::runtime_error("GLClasses: Trying to bind a second GLBuffer.");
        }
        _bound_buffer = _buffer.buffer();
        glBindBuffer(GL_ARRAY_BUFFER, _buffer.buffer());
    }
    ~GLBindBuffer() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        _bound_buffer = 0;
    }
    
    template<typename ...Types>
    void set(int first, int size, GLenum usage, const std::vector<Types>& ...attributes) {
        typedef std::tuple<Types...> TupleType;
        enum { stride = GLInternal::GLSum<GLInternal::GLBytePacker<Types>::size...>::value };
        std::tuple<const std::vector<Types>&...> attributes_reference{attributes...};
        std::vector<uint8_t> raw_data(stride * size);
        for (int i = 0; i < size; ++i) {
            GLInternal::GLListBytePacker<Types...>::pack(
                &raw_data[i * stride],
                attributes[first + i]...
            );
        }
        
        _buffer._size = size;
        _buffer._attribute_info.resize(std::tuple_size<TupleType>::value);
        GLInternal::GLGetAttributeInfoHelper<std::tuple_size<TupleType>::value-1, Types...>::get(_buffer._attribute_info);
        
        glBufferData(GL_ARRAY_BUFFER, raw_data.size(), &raw_data[0], usage);
    }
    
    size_t attribute_count() const {
        return _buffer.attribute_count();
    }
    GLint attribute_size(size_t attribute) const {
        return _buffer.attribute_size(attribute);
    }
    GLenum attribute_type(size_t attribute) const {
        return _buffer.attribute_type(attribute);
    }
    GLsizei attribute_stride(size_t attribute) const {
        return _buffer.attribute_stride(attribute);
    }
    GLsizei attribute_offset(size_t attribute) const {
        return _buffer.attribute_offset(attribute);
    }
    GLsizei size() const {
        return _buffer.size();
    }
    GLuint buffer() const {
        return _buffer.buffer();
    }
}; // GLBindBuffer

////////////////////////////////////////////////////////////////////////////////
// The OpenGL vertex array stores the state for vertex data

class GLBindVertexArray;

class GLVertexArray {
    GLuint _vertex_array;
    
    GLVertexArray(const GLVertexArray&);
    const GLVertexArray& operator = (const GLVertexArray&);
    
    GLsizei _size;
    std::vector<GLuint> _enabled_vertex_attrib_arrays;
    
public:
    friend class GLBindVertexArray;
    
    GLVertexArray() : _vertex_array(0), _size(0) {
        glGenVertexArrays(1, &_vertex_array);
    }
    GLVertexArray(GLVertexArray&& vertex_array) : _vertex_array(std::move(vertex_array._vertex_array)), _size(std::move(vertex_array._size)) {
        vertex_array._vertex_array = 0;
        vertex_array._size = 0;
    }
    ~GLVertexArray() {
        if (_vertex_array != 0) {
            glDeleteVertexArrays(1, &_vertex_array);
        }
    }
    const GLVertexArray& operator = (GLVertexArray&& vertex_array) {
        if (_vertex_array != 0) {
            glDeleteVertexArrays(1, &_vertex_array);
        }
        
        _vertex_array = std::move(vertex_array._vertex_array);
        _size = std::move(vertex_array._size);
        vertex_array._vertex_array = 0;
        vertex_array._size = 0;
        
        return *this;
    }
    
    GLsizei size() const {
        return _size;
    }
    
    GLuint vertex_array() const {
        return _vertex_array;
    }
}; // GLVertexArray

////////////////////////////////////////////////////////////////////////////////
// A helper to bind and manipulate the vertex array

class GLBindVertexArray : private GLInternal::GLBindBufferLock {
    GLVertexArray& _vertex_array;
    
    GLBindVertexArray(const GLBindVertexArray&);
    const GLBindVertexArray& operator = (const GLBindVertexArray&);
    
public:
    GLBindVertexArray(GLVertexArray& vertex_array) : _vertex_array(vertex_array) {
        if (_bound_vertex_array != 0) {
            throw std::runtime_error("GLClasses: Trying to bind a second GLVertexArray.");
        }
        if (_bound_buffer != 0) {
            throw std::runtime_error("GLClasses: Trying to bind a GLVertexArray while a GLBuffer is bound.");
        }
        
        _bound_vertex_array = _vertex_array.vertex_array();
        glBindVertexArray(_vertex_array.vertex_array());
    }
    ~GLBindVertexArray() {
        glBindVertexArray(0);
        _bound_vertex_array = 0;
    }
    
    void set(const GLBuffer& buffer, const std::vector<std::tuple<GLuint, size_t, bool>>& attribute_adaptor) {
        for (int i = 0; i < _vertex_array._enabled_vertex_attrib_arrays.size(); ++i) {
            glDisableVertexAttribArray(_vertex_array._enabled_vertex_attrib_arrays[i]);
        }
        
        glBindBuffer(GL_ARRAY_BUFFER, buffer.buffer());
        _vertex_array._size = buffer.size();
        _vertex_array._enabled_vertex_attrib_arrays.resize(attribute_adaptor.size());
        for (int i = 0; i < attribute_adaptor.size(); ++i) {
            if (buffer.attribute_count() <= std::get<0>(attribute_adaptor[i])) {
                throw std::runtime_error("GLClasses: Non existing attribute requested from GLBuffer.");
            }
            _vertex_array._enabled_vertex_attrib_arrays[i] = std::get<0>(attribute_adaptor[i]);
            glEnableVertexAttribArray(std::get<0>(attribute_adaptor[i]));
            glVertexAttribPointer(std::get<0>(attribute_adaptor[i]),
                                  buffer.attribute_size(std::get<1>(attribute_adaptor[i])),
                                  buffer.attribute_type(std::get<1>(attribute_adaptor[i])),
                                  std::get<2>(attribute_adaptor[i]) ? GL_TRUE : GL_FALSE,
                                  buffer.attribute_stride(std::get<1>(attribute_adaptor[i])),
                                  (GLvoid*)((uint8_t*)0 + buffer.attribute_offset(std::get<1>(attribute_adaptor[i]))));
        }
    }
    
    void draw(GLenum mode, GLint first = 0, GLsizei count = 0) {
        if (count == 0) {
            count = _vertex_array.size();
        } else {
            if (count >= _vertex_array.size() - first) {
                throw std::runtime_error("GLClasses: Trying to draw more vertices than available in the bound GLBuffer.");
            }
        }
        glDrawArrays(mode, first, count);
    }
}; // GLBindVertexArray

////////////////////////////////////////////////////////////////////////////////
// The OpenGL shader program

class GLProgram {
    GLuint _program;
    
    void compile_shader(GLuint shader_identifier, const std::string& shader_source) {
        GLint shader_ok = 0;
        
        // compile the vertex shader
        const char* c_source = shader_source.c_str();
        glShaderSource(shader_identifier, 1, &c_source, 0);
        glCompileShader(shader_identifier);
        glGetShaderiv(shader_identifier, GL_COMPILE_STATUS, &shader_ok);
        if (!shader_ok) {
            GLint log_length;
            glGetShaderiv(shader_identifier, GL_INFO_LOG_LENGTH, &log_length);
            char *log = new char[log_length];
            glGetShaderInfoLog(shader_identifier, log_length, 0, log);
            // TODO: Change this to exceptions.
            std::cout << shader_source << std::endl;
            std::cout << "ERROR: Unable to compile shader:" << std::endl;
            std::cout << log << std::endl;
            delete [] log;
        }
    }
    
    void link_program() {
        glLinkProgram(_program);
        GLint program_ok;
        glGetProgramiv(_program, GL_LINK_STATUS, &program_ok);
        if (!program_ok) {
            GLint log_length;
            glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &log_length);
            char* log = new char[log_length];
            glGetProgramInfoLog(_program, log_length, 0, log);
            // TODO: Change this to exceptions.
            std::cout << "ERROR: Unable to link program" << std::endl;
            std::cout << log << std::endl;
            delete [] log;
        }
    }
    
    GLProgram(const GLProgram&);
    const GLProgram& operator = (const GLProgram&);
    
public:
    GLProgram() : _program(0) {
        _program = glCreateProgram();
    }
    GLProgram(GLProgram&& program) : _program(std::move(program._program)) {
        program._program = 0;
    }
    ~GLProgram() {
        if (_program != 0) {
            glDeleteProgram(_program);
        }
    }
    const GLProgram& operator = (GLProgram&& program) {
        if (_program != 0) {
            glDeleteProgram(_program);
        }
        
        _program = std::move(program._program);
        program._program = 0;
        return *this;
    }
    
    void load_shaders(const std::string& vertex_shader_source,
                      const std::string& fragment_shader_source,
                      const std::vector<std::string> attribute_names,
                      const std::vector<std::string> frag_data_names) {
        GLuint vertex_shader_identifier = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragment_shader_identifier = glCreateShader(GL_FRAGMENT_SHADER);
        
        compile_shader(vertex_shader_identifier, vertex_shader_source);
        compile_shader(fragment_shader_identifier, fragment_shader_source);
        
        glAttachShader(_program, vertex_shader_identifier);
        glAttachShader(_program, fragment_shader_identifier);
        
        for (GLuint index = 0; index < attribute_names.size(); ++index) {
            glBindAttribLocation(_program, index, attribute_names[index].c_str());
        }
        for (GLuint index = 0; index < frag_data_names.size(); ++index) {
            glBindFragDataLocation(_program, index, frag_data_names[index].c_str());
        }
        
        link_program();
        
        glDetachShader(_program, vertex_shader_identifier);
        glDetachShader(_program, fragment_shader_identifier);
        
        glDeleteShader(fragment_shader_identifier);
        glDeleteShader(vertex_shader_identifier);
    }
    
    GLint attribute_location(const std::string& attribute_name) const {
        return glGetAttribLocation(_program, attribute_name.c_str());
    }
    GLint uniform_location(const std::string& uniform_name) const {
        return glGetUniformLocation(_program, uniform_name.c_str());
    }
    GLint frag_data_location(const std::string& frag_data_name) const {
        return glGetFragDataLocation(_program, frag_data_name.c_str());
    }
    
    GLuint program() const {
        return _program;
    }
}; // GLProgram

namespace GLInternal {
    struct GLBindProgramLock {
        static GLuint _bound_program;
    }; // GLBindProgramLock
    
    template<typename Type>
    void set_uniform(GLint location, const Type& value, bool transpose = false);
}

////////////////////////////////////////////////////////////////////////////////
// A helper to use a program and to manipulate its uniforms

class GLBindProgram : private GLInternal::GLBindProgramLock {
    GLProgram& _program;
    
    GLBindProgram(const GLBindProgram&);
    const GLBindProgram& operator = (const GLBindProgram&);
    
public:
    GLBindProgram(GLProgram& program) : _program(program) {
        if (_bound_program != 0) {
            throw std::runtime_error("GLClasses: Trying to bind a second GLProgram.");
        }
        _bound_program = _program.program();
        glUseProgram(_program.program());
    }
    ~GLBindProgram() {
        glUseProgram(0);
        _bound_program = 0;
    }
    
    template<typename Type>
    void set_uniform(GLint location, const Type& value, bool transpose = false) const {
        GLInternal::set_uniform(location, value, transpose);
    }
}; // GLBindProgram

////////////////////////////////////////////////////////////////////////////////
// The OpenGL texture resource

class GLTexture {
    GLuint _texture;
    
    GLTexture(const GLTexture&);
    const GLTexture& operator = (const GLTexture&);
    
public:
    GLTexture() : _texture(0) {
        glGenTextures(1, &_texture);
    }
    GLTexture(GLTexture&& texture) : _texture(std::move(texture._texture)) {
        texture._texture = 0;
    }
    ~GLTexture() {
        if (_texture != 0) {
            glDeleteTextures(1, &_texture);
        }
    }
    const GLTexture& operator = (GLTexture&& texture) {
        if (_texture != 0) {
            glDeleteTextures(1, &_texture);
        }
        
        _texture = std::move(texture._texture);
        texture._texture = 0;
        return *this;
    }
    
    GLuint texture() const {
        return _texture;
    }
}; // GLTexture

namespace GLInternal {
    template<GLenum active_texture>
    struct GLBindTextureLock {
        static GLuint _bound_texture;
    }; // GLBindTextureLock
}

////////////////////////////////////////////////////////////////////////////////
// A helper to bind and manipulate textures. Only one texture may be bound to
// each texture channel at a time.

template <GLenum active_texture>
class GLBindTexture : private GLInternal::GLBindTextureLock<active_texture> {
    GLTexture& _texture;
    
    GLBindTexture(const GLBindTexture&);
    const GLBindTexture& operator = (const GLBindTexture&);
    
    /*template<typename ColorType>
    std::vector<uint8_t> pack(const Image<ColorType>& image_data) {
        enum {pixel_size = GLInternal::GLBytePacker<ColorType>::size};
        std::vector<uint8_t> result(image_data.width() * image_data.height() * pixel_size);
        for (int y = 0; y < image_data.height(); ++y) {
            for (int x = 0; x < image_data.width(); ++x) {
                GLInternal::GLBytePacker<ColorType>::pack(&result[(y * image_data.width() + x) * pixel_size], image_data(x, y));
            }
        }
        return result;
    }*/
    
    template<typename ColorType>
    std::vector<uint8_t> pack(std::vector<ColorType> const& data) {
        enum {pixel_size = GLInternal::GLBytePacker<ColorType>::size};
        std::vector<uint8_t> result(data.size() * pixel_size);
        for (int i = 0; i < data.size(); ++i) {
                GLInternal::GLBytePacker<ColorType>::pack(&result[i * pixel_size], data[i]);
        }
        return result;
    }
    
public:
    GLBindTexture(GLTexture& texture) : _texture(texture) {
        if (GLInternal::GLBindTextureLock<active_texture>::_bound_texture != 0) {
            throw std::runtime_error("GLClasses: Trying to bind a second GLTexture to the same texture channel.");
        }
        GLInternal::GLBindTextureLock<active_texture>::_bound_texture = _texture.texture();
        glActiveTexture(active_texture);
        
        glBindTexture(GL_TEXTURE_2D, _texture.texture());
    }
    ~GLBindTexture() {
        glActiveTexture(active_texture);
        
        glBindTexture(GL_TEXTURE_2D, 0);
        GLInternal::GLBindTextureLock<active_texture>::_bound_texture = 0;
    }
    
    void set_parameter(GLenum pname, GLint param) {
        glActiveTexture(active_texture);
        
        glTexParameteri(GL_TEXTURE_2D, pname, param);
    }
    
    void set(GLsizei width, GLsizei height, GLint internalformat, GLint level = 0) {
        glActiveTexture(active_texture);
        
        GLenum format = GL_RGBA;
        GLenum type = GL_UNSIGNED_BYTE;
        
        glTexImage2D(GL_TEXTURE_2D, level, internalformat, width, height, 0, format, type, 0);
    }
    
    void set(GLsizei width, GLsizei height, GLint internalformat, GLenum format, GLenum type, GLint level = 0) {
        glActiveTexture(active_texture);
        
        glTexImage2D(GL_TEXTURE_2D, level, internalformat, width, height, 0, format, type, 0);
    }
    
    /*template<typename ColorType>
    void set(const Image<ColorType>& image_data, GLint internalformat, GLint level = 0) {
        glActiveTexture(active_texture);
        
        std::vector<uint8_t> raw_data = pack(image_data);
        
        GLenum format = GLInternal::GLColorType<ColorType>::format;
        GLenum type = GLInternal::GLColorType<ColorType>::type;
        
        glTexImage2D(GL_TEXTURE_2D, level, internalformat, image_data.width(), image_data.height(), 0, format, type, &raw_data[0]);
    }*/
    
    template<typename ColorType>
    void set(std::vector<ColorType> const& data,
             GLint width, GLint height,
             GLint internalformat, GLint level = 0) {
        glActiveTexture(active_texture);
        
        std::vector<uint8_t> raw_data = pack(data);
        
        GLenum format = GLInternal::GLColorType<ColorType>::format;
        GLenum type = GLInternal::GLColorType<ColorType>::type;
        
        glTexImage2D(GL_TEXTURE_2D, level, internalformat, width, height, 0, format, type, &raw_data[0]);
    }
}; // GLBindTexture

////////////////////////////////////////////////////////////////////////////////
// The OpenGL renderbuffer resource.

class GLRenderbuffer {
    GLuint _renderbuffer;
    
    GLRenderbuffer(const GLRenderbuffer&);
    const GLRenderbuffer& operator = (const GLRenderbuffer&);
    
public:
    GLRenderbuffer() : _renderbuffer(0) {
        glGenRenderbuffers(1, &_renderbuffer);
    }
    GLRenderbuffer(GLRenderbuffer&& renderbuffer) : _renderbuffer(std::move(renderbuffer._renderbuffer)) {
        renderbuffer._renderbuffer = 0;
    }
    ~GLRenderbuffer() {
        if (_renderbuffer != 0) {
            glDeleteRenderbuffers(1, &_renderbuffer);
        }
    }
    const GLRenderbuffer& operator = (GLRenderbuffer&& renderbuffer) {
        if (_renderbuffer != 0) {
            glDeleteRenderbuffers(1, &_renderbuffer);
        }
        
        _renderbuffer = renderbuffer._renderbuffer;
        renderbuffer._renderbuffer = 0;
        
        return *this;
    }
    
    GLuint renderbuffer() const {
        return _renderbuffer;
    }
}; // GLRenderbuffer

namespace GLInternal {
    struct GLBindRenderbufferLock {
        static GLuint _bound_renderbuffer;
    }; // GLBindRenderbufferLock
}

////////////////////////////////////////////////////////////////////////////////
// A helper to bind and manipulate the renderbuffer.

class GLBindRenderbuffer : private GLInternal::GLBindRenderbufferLock {
    GLRenderbuffer& _renderbuffer;
    
public:
    GLBindRenderbuffer(GLRenderbuffer& renderbuffer) : _renderbuffer(renderbuffer) {
        if (_bound_renderbuffer != 0) {
            throw std::runtime_error("GLClasses: Trying to bind a second GLRenderbuffer.");
        }
        glBindRenderbuffer(GL_RENDERBUFFER, _renderbuffer.renderbuffer());
    }
    ~GLBindRenderbuffer() {
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }
    
    void set(GLsizei width, GLsizei height, GLenum internalformat) {
        glRenderbufferStorage(GL_RENDERBUFFER, internalformat, width, height);
    }
}; // GLBindRenderbuffer

////////////////////////////////////////////////////////////////////////////////
// The OpenGL framebuffer resource.

class GLFramebuffer {
    GLuint _framebuffer;
    
    GLFramebuffer(const GLFramebuffer&);
    const GLFramebuffer& operator = (const GLFramebuffer&);
    
public:
    GLFramebuffer() : _framebuffer(0) {
        glGenFramebuffers(1, &_framebuffer);
    }
    GLFramebuffer(GLFramebuffer&& framebuffer) : _framebuffer(std::move(framebuffer._framebuffer)) {
        framebuffer._framebuffer = 0;
    }
    ~GLFramebuffer() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        if (_framebuffer != 0) {
            glDeleteFramebuffers(1, &_framebuffer);
        }
    }
    const GLFramebuffer& operator = (GLFramebuffer&& framebuffer) {
        _framebuffer = std::move(framebuffer._framebuffer);
        framebuffer._framebuffer = 0;
        return *this;
    }
    
    GLuint framebuffer() const {
        return _framebuffer;
    }
}; // GLFramebuffer

namespace GLInternal {
    struct GLBindFramebufferLock {
        static GLuint _bound_framebuffer;
    }; // GLBindFramebufferLock
}

////////////////////////////////////////////////////////////////////////////////
// A helper to bind and manipulate a framebuffer.

class GLBindFramebuffer : private GLInternal::GLBindFramebufferLock {
    GLBindFramebuffer(const GLBindFramebuffer&);
    const GLBindFramebuffer& operator = (const GLBindFramebuffer&);
    
    GLFramebuffer& _framebuffer;
    
public:
    GLBindFramebuffer(GLFramebuffer& framebuffer) : _framebuffer(framebuffer) {
        if (_bound_framebuffer != 0) {
            throw std::runtime_error("GLClasses: Trying to bind a second GLFramebuffer.");
        }
        _bound_framebuffer = _framebuffer.framebuffer();
        
        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer.framebuffer());
    }
    ~GLBindFramebuffer() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        _bound_framebuffer = 0;
    }
    
    void set(const GLTexture& texture, GLenum attachment, GLint level = 0) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture.texture(), level);
    }
    
    void set(const GLRenderbuffer& renderbuffer, GLenum attachment) {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderbuffer.renderbuffer());
    }
}; // GLBindFramebuffer

////////////////////////////////////////////////////////////////////////////////

#endif /* defined(__ComponentBasedGame__GLClasses__) */

////////////////////////////////////////////////////////////////////////////////
