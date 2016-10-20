#pragma once
#include "GLTexture.h"
#include "TexParams.h"

#include <string>

namespace Sakura {

    //Loads images into GLTextures
    class ImageLoader
    {
    public:
        static GLTexture loadPNG(std::string filePath, TexParam param);
    };

}