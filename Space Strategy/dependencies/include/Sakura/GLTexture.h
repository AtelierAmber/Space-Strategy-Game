#pragma once
#include <GL/glew.h>

namespace Sakura {

    class GLTexture {
	public:
		GLTexture(){ }
        GLuint id;
        int width;
        int height;
    };
}