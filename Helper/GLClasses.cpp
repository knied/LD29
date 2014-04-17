//
//  GLClasses.cpp
//  ComponentBasedGame
//
//  Created by Kristof Niederholtmeyer on 02.10.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "GLClasses.h"

////////////////////////////////////////////////////////////////////////////////

namespace GLInternal {
    template<>
    void set_uniform<float>(GLint location,
                            const float& v,
                            bool transpose) {
        glUniform1f(location, v);
    }
    
    template<>
    void set_uniform<Vector<float,1>>(GLint location,
                                      const Vector<float,1>& v,
                                      bool transpose) {
        glUniform1fv(location, 1, &v[0]);
    }
    template<>
    void set_uniform<Vector<float,2>>(GLint location,
                                      const Vector<float,2>& v,
                                      bool transpose) {
        glUniform2fv(location, 1, &v[0]);
    }
    template<>
    void set_uniform<Vector<float,3>>(GLint location,
                                      const Vector<float,3>& v,
                                      bool transpose) {
        glUniform3fv(location, 1, &v[0]);
    }
    template<>
    void set_uniform<Vector<float,4>>(GLint location,
                                      const Vector<float,4>& v,
                                      bool transpose) {
        glUniform4fv(location, 1, &v[0]);
    }
    
    template<>
    void set_uniform<int>(GLint location,
                          const int& v,
                          bool transpose) {
        glUniform1i(location, v);
    }
    
    template<>
    void set_uniform<Vector<int,1>>(GLint location,
                                    const Vector<int,1>& v,
                                    bool transpose) {
        glUniform1iv(location, 1, &v[0]);
    }
    template<>
    void set_uniform<Vector<int,2>>(GLint location,
                                    const Vector<int,2>& v,
                                    bool transpose) {
        glUniform2iv(location, 1, &v[0]);
    }
    template<>
    void set_uniform<Vector<int,3>>(GLint location,
                                    const Vector<int,3>& v,
                                    bool transpose) {
        glUniform3iv(location, 1, &v[0]);
    }
    template<>
    void set_uniform<Vector<int,4>>(GLint location,
                                    const Vector<int,4>& v,
                                    bool transpose) {
        glUniform4iv(location, 1, &v[0]);
    }
    
    template<>
    void set_uniform<Matrix<float,2,2>>(GLint location,
                                        const Matrix<float,2,2>& v,
                                        bool transpose) {
        glUniformMatrix2fv(location, 1, transpose ? GL_TRUE : GL_FALSE, &v(0,0));
    }
    template<>
    void set_uniform<Matrix<float,3,3>>(GLint location,
                                        const Matrix<float,3,3>& v,
                                        bool transpose) {
        glUniformMatrix3fv(location, 1, transpose ? GL_TRUE : GL_FALSE, &v(0,0));
    }
    template<>
    void set_uniform<Matrix<float,4,4>>(GLint location,
                                        const Matrix<float,4,4>& v,
                                        bool transpose) {
        glUniformMatrix4fv(location, 1, transpose ? GL_TRUE : GL_FALSE, &v(0,0));
    }
    
    GLuint GLBindBufferLock::_bound_buffer = 0;
    GLuint GLBindBufferLock::_bound_vertex_array = 0;
    GLuint GLBindProgramLock::_bound_program = 0;
    template<> GLuint GLBindTextureLock<GL_TEXTURE0>::_bound_texture = 0;
    template<> GLuint GLBindTextureLock<GL_TEXTURE1>::_bound_texture = 0;
    template<> GLuint GLBindTextureLock<GL_TEXTURE2>::_bound_texture = 0;
    template<> GLuint GLBindTextureLock<GL_TEXTURE3>::_bound_texture = 0;
    template<> GLuint GLBindTextureLock<GL_TEXTURE4>::_bound_texture = 0;
    template<> GLuint GLBindTextureLock<GL_TEXTURE5>::_bound_texture = 0;
    template<> GLuint GLBindTextureLock<GL_TEXTURE6>::_bound_texture = 0;
    template<> GLuint GLBindTextureLock<GL_TEXTURE7>::_bound_texture = 0;
    template<> GLuint GLBindTextureLock<GL_TEXTURE8>::_bound_texture = 0;
    template<> GLuint GLBindTextureLock<GL_TEXTURE9>::_bound_texture = 0;
    template<> GLuint GLBindTextureLock<GL_TEXTURE10>::_bound_texture = 0;
    template<> GLuint GLBindTextureLock<GL_TEXTURE11>::_bound_texture = 0;
    template<> GLuint GLBindTextureLock<GL_TEXTURE12>::_bound_texture = 0;
    template<> GLuint GLBindTextureLock<GL_TEXTURE13>::_bound_texture = 0;
    template<> GLuint GLBindTextureLock<GL_TEXTURE14>::_bound_texture = 0;
    template<> GLuint GLBindTextureLock<GL_TEXTURE15>::_bound_texture = 0;
    template<> GLuint GLBindTextureLock<GL_TEXTURE16>::_bound_texture = 0;
    template<> GLuint GLBindTextureLock<GL_TEXTURE17>::_bound_texture = 0;
    template<> GLuint GLBindTextureLock<GL_TEXTURE18>::_bound_texture = 0;
    template<> GLuint GLBindTextureLock<GL_TEXTURE19>::_bound_texture = 0;
    template<> GLuint GLBindTextureLock<GL_TEXTURE20>::_bound_texture = 0;
    template<> GLuint GLBindTextureLock<GL_TEXTURE21>::_bound_texture = 0;
    template<> GLuint GLBindTextureLock<GL_TEXTURE22>::_bound_texture = 0;
    template<> GLuint GLBindTextureLock<GL_TEXTURE23>::_bound_texture = 0;
    template<> GLuint GLBindTextureLock<GL_TEXTURE24>::_bound_texture = 0;
    template<> GLuint GLBindTextureLock<GL_TEXTURE25>::_bound_texture = 0;
    template<> GLuint GLBindTextureLock<GL_TEXTURE26>::_bound_texture = 0;
    template<> GLuint GLBindTextureLock<GL_TEXTURE27>::_bound_texture = 0;
    template<> GLuint GLBindTextureLock<GL_TEXTURE28>::_bound_texture = 0;
    template<> GLuint GLBindTextureLock<GL_TEXTURE29>::_bound_texture = 0;
    template<> GLuint GLBindTextureLock<GL_TEXTURE30>::_bound_texture = 0;
    template<> GLuint GLBindTextureLock<GL_TEXTURE31>::_bound_texture = 0;
    GLuint GLBindRenderbufferLock::_bound_renderbuffer = 0;
    GLuint GLBindFramebufferLock::_bound_framebuffer = 0;
    
}

////////////////////////////////////////////////////////////////////////////////
