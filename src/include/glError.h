#include <GL/glew.h>
#include <iostream>
inline void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

inline bool GLLogCall(const char* function, const char* file, int line)
{
    bool success = true;

    while (GLenum error = glGetError())
    {
        std::cerr << "[OpenGL Error] ("<< error << std::dec<< "), Function: " << function
                  << "\nFile: " << file
                  << ", Line: " << line
                  << "\n\n";

        success = false;
    }

    return success;
}

#ifdef _DEBUG

#include <intrin.h>

#define ENGINE_ASSERT(condition)        \
    do                                  \
    {                                   \
        if (!(condition))               \
        {                               \
            __debugbreak();             \
        }                               \
    } while (0)

#else

#define ENGINE_ASSERT(condition) ((void)0)

#endif

#ifdef _DEBUG
#define glCall(x)            \
    GLClearError();          \
    x;                       \
    ENGINE_ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
#define glCall(x) x
#endif
