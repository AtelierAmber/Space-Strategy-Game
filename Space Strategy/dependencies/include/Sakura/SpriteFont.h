/* 
    This is a modified version of the SpriteFont class from the
    Seed Of Andromeda source code.
    Use it for any of your projects, commercial or otherwise,
    free of charge, but do not remove this disclaimer.

    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS
    ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO
    EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
    INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
    RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
    ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
    OF THIS SOFTWARE.

    November 28 2014
    Original Author: Cristian Zaloj
    Modified By: Benjamin Arnold
*/

#pragma once

#ifndef SpriteFont_h__
#define SpriteFont_h__

#include <TTF\SDL_ttf.h>
#include <glm\glm.hpp>
#include <map>
#include <vector>
#include "TexParams.h"

#include "Vertex.h"

namespace Sakura {

    class GLTexture;
    class SpriteBatch;

    struct CharGlyph {
    public:
        char character;
        glm::vec4 uvRect;
        glm::vec2 size;
    };

#define FIRST_PRINTABLE_CHAR ((char)32)
#define LAST_PRINTABLE_CHAR ((char)126)

    /// For text justification
    enum class Justification {
        LEFT, MIDDLE, RIGHT
    };

    class SpriteFont {
    public:
        SpriteFont();
		~SpriteFont();

		/* TTF Font */
		void initTTF(const char* font, int size, char cs, char ce, TexParam param = DEFAULT);
		void initTTF(const char* font, int size, TexParam param = DEFAULT){
			initTTF(font, size, FIRST_PRINTABLE_CHAR, LAST_PRINTABLE_CHAR, param);
		}
		
		/* PNG Font */ 
		/* Must be formatted like such in file
		 * 
		 *  !"#$%&'()*+,-./
		 * 0123456789:;<=>?
		 * @ABCDEFGHIJKLMNO
		 * PQRSTUVWXYZ[\]^_
		 * 'abcdefghijklmno
		 * pqrstuvwxyz{|}~
		 * 
		 * WARNING:
		 * If a symbol space is left blank it will 
		 * be blank when printed
		 * Can use diferent sized symbolsheets as long as you keep the symbols in order */
		void initPNG(const char* font, const glm::ivec2& size, char cs, char ce, TexParam param = DEFAULT);
		void initPNG(const char* font, const glm::ivec2& size, TexParam param = DEFAULT){
			initPNG(font, size, FIRST_PRINTABLE_CHAR, LAST_PRINTABLE_CHAR, param);
		}
 
 		

        /* Destroys the font resources */
        void dispose();

        int getFontHeight() const {
            return m_fontHeight;
        }

        /* Measures the dimensions of the text */
        glm::vec2 measure(const char* s);

        /* Draws using a spritebatch */
        void draw(SpriteBatch& batch, const char* s, glm::vec2 position, glm::vec2 scaling, 
                  float depth, ColorRGBA8 tint, Justification just = Justification::LEFT);
    private:
        static std::vector<int>* createRows(glm::ivec4* rects, int rectsLength, int r, int padding, int& w);

        int m_regStart, m_regLength;
        CharGlyph* m_glyphs;
        int m_fontHeight;

        unsigned int m_texID;
    };

}

#endif // SpriteFont_h__