#pragma once
#include "ResourceManager.h"
#include <GL/glew.h>
#include "GLTexture.h"

namespace Sakura {

    //A 2D quad that can be rendered to the screen
    class Sprite
    {
    public:
        Sprite();
        ~Sprite();

        void init(float x, float y, float width, float height, const char* texturePath, ResourceManager* resouceManager);

        void draw();

        //private:
        float m_x;
        float m_y;
        float m_width;
        float m_height;
        GLuint m_vboID;
        GLTexture m_texture;
    };

}