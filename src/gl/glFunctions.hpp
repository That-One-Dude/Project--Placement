/******************************************************************************
* File: glFunctions.hpp
* Author: Tony Skeps
* Date: June 9, 2022
******************************************************************************/

#include <SFML/OpenGL.hpp>

#ifndef GL_FUNCTIONS_HPP
#define GL_FUNCTIONS_HPP

namespace Game::GL {

    /*// General Types
    typedef void (*GL_NO_PARAMETER_FUNCTION) ();
    typedef void (*GL_GEN_FUNCTION) (GLsizei, GLuint*);
    typedef void (*GL_BIND_FUNCTION) (GLuint);
    typedef void (*GL_MODED_BIND_FUNCTION) (GLenum, GLuint);
    typedef void (*GL_MODED_BIND_INDEX_FUNCTION) (GLenum, GLuint, GLuint);

    // VBO stuff
    const extern GL_GEN_FUNCTION glGenBuffers;
    const extern GL_MODED_BIND_FUNCTION glBindBuffer;
    const extern GL_MODED_BIND_INDEX_FUNCTION glBindBufferBase;
    typedef void (*GL_BUFFER_DATA) (GLenum, GLsizeiptr, const void*, GLenum);
    const extern GL_BUFFER_DATA glBufferData;
    typedef void (*GL_BUFFER_SUB_DATA) (GLenum, GLintptr, GLsizeiptr, const void*);
    const extern GL_BUFFER_SUB_DATA glBufferSubData;
    const extern GL_GEN_FUNCTION glDeleteBuffers;

    // VAO stuff
    const extern GL_GEN_FUNCTION glGenVertexArrays;
    const extern GL_BIND_FUNCTION glBindVertexArray;
    const extern GL_GEN_FUNCTION glDeleteVertexArrays;

    // Program Stuff
    typedef GLuint (*GL_CREATE_PROGRAM) ();
    const extern GL_CREATE_PROGRAM glCreateProgram;
    const extern GL_BIND_FUNCTION glUseProgram;
    const extern GL_BIND_FUNCTION glLinkProgram;

    // Shader Stuff
    typedef GLuint (*GL_CREATE_SHADER) (GLenum);
    const extern GL_CREATE_SHADER glCreateShader;
    typedef void (*GL_SHADER_SOURCE)(GLuint, GLsizei, const GLchar**, const GLint*);
    const extern GL_SHADER_SOURCE glShaderSource;
    const extern GL_BIND_FUNCTION glCompileShader;
    typedef void (*GL_ATTACH_SHADER) (GLuint, GLuint);
    const extern GL_ATTACH_SHADER glAttachShader;
    const extern GL_BIND_FUNCTION glDeleteShader;

    // Uniforms
    typedef GLint (*GL_GET_UNIFORM_LOCATION) (GLuint, const GLchar*);
    const extern GL_GET_UNIFORM_LOCATION glGetUniformLocation;
    typedef void (*GL_FLOAT_UNIFORM) (GLint, GLsizei, GLboolean, const GLfloat*);
    const extern GL_FLOAT_UNIFORM glUniformMatrix4fv;

    // Other stuff
    typedef void (*GL_VERTEX_ATTRIB_POINTER) (GLuint, GLint, GLenum, GLboolean, GLsizei, const void*);
    const extern GL_VERTEX_ATTRIB_POINTER glVertexAttribPointer;
    typedef void (*GL_ENABLE_VERTEX_ATTRIB_ARRAY) (GLuint);
    const extern GL_ENABLE_VERTEX_ATTRIB_ARRAY glEnableVertexAttribArray;*/

    #define GL_FUNCTION(type, name) const extern type name;
    #include "glFunctionList.hpp"

}

#endif