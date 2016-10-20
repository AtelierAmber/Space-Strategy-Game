#pragma once

#include <string>
#include <GL/glew.h>

namespace Sakura {

    //This class handles the compilation, linking, and usage of a GLSL shader program.
    //Reference: http://www.opengl.org/wiki/Shader_Compilation
    class GLSLProgram
    {
    public:
        GLSLProgram();
        ~GLSLProgram();

        void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilepath);

		void compileShadersFromSource(const char* vertexSource, const char* fragmentSource);

        void linkShaders();

        void addAttribute(const std::string& attributeName);

        GLint getUniformLocation(const std::string& uniformName);

        void use();
        void unuse();

		void dispose();
    private:

        int m_numAttributes;

		void loadFile();
		void compileShader(const char* source, const std::string& name, GLuint id);

        GLuint m_programID;

        GLuint m_vertexShaderID;
        GLuint m_fragmentShaderID;
    };

}
