#pragma once
#include "TextureCache.h"
#include "TileSheet.h"
#include "AudioEngine.h"

namespace Sakura {

    //This is a way for us to access all our resources, such as
    //Textures and Audio
    class ResourceManager
    {
    public:
		void init();

		TileSheet getTileSheet(const char* tileSheetPath, const glm::ivec2& tileDims, TexParam textureFlags = DEFAULT);
		GLTexture getTexture(const char* texturePath, TexParam textureFlags = DEFAULT);
		SoundEffect loadSoundEffect(const char* effectPath);
		Music loadMusic(const char* musicPath);

		void destroyResources();

    private:
        TextureCache m_textureCache;
		AudioEngine m_audioEngine;
    };
}