/******************************************************************************
* File: glFunctions.hpp
* Author: Tony Skeps
* Date: June 9, 2022
******************************************************************************/

#include "glFunctions.hpp"

////////////////////////////////////////////////////////////
/// This just includes the OpenGL headers,
/// which have actually different paths on each system
////////////////////////////////////////////////////////////
#if defined(SFML_SYSTEM_WINDOWS)

    // The Visual C++ version of gl.h uses WINGDIAPI and APIENTRY but doesn't define them
    #ifdef _MSC_VER
        #include <windows.h>
    #endif

    #include <GL/glx.h>

#elif defined(SFML_SYSTEM_LINUX) || defined(SFML_SYSTEM_FREEBSD) || defined(SFML_SYSTEM_OPENBSD)

    #if defined(SFML_OPENGL_ES)
        #include <GLES/glx.h>
    #else
        #include <GL/glx.h>
        #define GL_LOAD_FUNCTION(type, name) (type)(glXGetProcAddressARB((GLubyte*)#name))
    #endif

#elif defined(SFML_SYSTEM_MACOS)

    #include <OpenGL/glx.h>

#elif defined (SFML_SYSTEM_IOS)

    #include <OpenGLES/ES1/glx.h>

#elif defined (SFML_SYSTEM_ANDROID)

    #include <GLES/glx.h>

#endif

namespace Game::GL {

    /*// VBO stuff
    GL_LOAD_FUNCTION(GL_GEN_FUNCTION, glGenBuffers);
    GL_LOAD_FUNCTION(GL_MODED_BIND_FUNCTION, glBindBuffer);
    GL_LOAD_FUNCTION(GL_MODED_BIND_INDEX_FUNCTION, glBindBufferBase);
    GL_LOAD_FUNCTION(GL_BUFFER_DATA, glBufferData);
    GL_LOAD_FUNCTION(GL_BUFFER_SUB_DATA, glBufferSubData);
    GL_LOAD_FUNCTION(GL_GEN_FUNCTION, glDeleteBuffers);

    // VAO stuff
    GL_LOAD_FUNCTION(GL_GEN_FUNCTION, glGenVertexArrays);
    GL_LOAD_FUNCTION(GL_BIND_FUNCTION, glBindVertexArray);
    GL_LOAD_FUNCTION(GL_GEN_FUNCTION, glDeleteVertexArrays);

    // Program Stuff
    GL_LOAD_FUNCTION(GL_CREATE_PROGRAM, glCreateProgram);
    GL_LOAD_FUNCTION(GL_BIND_FUNCTION, glUseProgram);
    GL_LOAD_FUNCTION(GL_BIND_FUNCTION, glLinkProgram);

    // Shader Stuff
    GL_LOAD_FUNCTION(GL_CREATE_SHADER, glCreateShader);
    GL_LOAD_FUNCTION(GL_SHADER_SOURCE, glShaderSource);
    GL_LOAD_FUNCTION(GL_BIND_FUNCTION, glCompileShader);
    GL_LOAD_FUNCTION(GL_ATTACH_SHADER, glAttachShader);
    GL_LOAD_FUNCTION(GL_BIND_FUNCTION, glDeleteShader);

    // Uniforms
    GL_LOAD_FUNCTION(GL_GET_UNIFORM_LOCATION, glGetUniformLocation);
    GL_LOAD_FUNCTION(GL_FLOAT_UNIFORM, glUniformMatrix4fv);

    // Other Stuff
    GL_LOAD_FUNCTION(GL_VERTEX_ATTRIB_POINTER, glVertexAttribPointer);
    GL_LOAD_FUNCTION(GL_ENABLE_VERTEX_ATTRIB_ARRAY, glEnableVertexAttribArray);*/

    #define GL_FUNCTION(type, name) const type name = GL_LOAD_FUNCTION(type, name);
    #include "glFunctionList.hpp"

}