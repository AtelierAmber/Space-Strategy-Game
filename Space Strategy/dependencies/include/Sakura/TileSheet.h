#pragma once

#include "GLTexture.h"

#include <glm\glm.hpp>

namespace Sakura{

	class TileSheet{
	public:
		TileSheet(){ }

		void init(const GLTexture& texture, const glm::ivec2& tileDims){
			this->texture = texture;
			this->dims = tileDims;
		}

		glm::vec4 getUVs(int index){
			int xTile = index % dims.x;
			int yTile = index / dims.x;
			yTile = dims.y - yTile - 1;

			glm::vec4 uv;
			uv.x = xTile / (float)dims.x;
			uv.y = yTile / (float)dims.y;
			uv.z = 1.0f / dims.x;
			uv.w = 1.0f / dims.y;

			return uv;
		}

		GLTexture texture;
		glm::ivec2 dims;
	};

}