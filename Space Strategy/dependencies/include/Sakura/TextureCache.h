#pragma once
#include <map>
#include <string>
#include "GLTexture.h"
#include "TexParams.h"

namespace Sakura {

    //This caches the textures so that multiple sprites can use the same textures
    class TextureCache
    {
    public:
        TextureCache();
        ~TextureCache();

		GLTexture getTexture(const char* texturePath, TexParam textureFlags);

		void destroy(){
			m_textureMap.clear();
		}

    private:
        std::map<std::string, GLTexture> m_textureMap;
    };

}