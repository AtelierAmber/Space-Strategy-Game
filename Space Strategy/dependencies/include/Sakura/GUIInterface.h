#pragma once
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "GLSLProgram.h"
#include "Camera2D.h"
#include "TexParams.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "IScreen.h"
#include "Bounds.h"
#include <functional>

namespace Sakura{

	struct GUIButton{
	public:
#define button_pressed_text_offset 0.0f
#define button_text_Y_padding 5.0f
#define button_text_X_padding 2.5f
		GUIButton(){}
		GUIButton(const char* texturePath, const char* fontPath, int fontSize, glm::vec2& textScale, TexParam fontParams, std::string content, glm::vec4 destRect, std::function<void()> calltoFunction, ResourceManager& resourceManager, TexParam texParams){
			init(texturePath, fontPath, fontSize, textScale, fontParams, content, destRect, calltoFunction, resourceManager, texParams);
		}
		GUIButton(const char* texturePath, const char* fontPath, int fontSize, glm::vec2& textScale, TexParam fontParams, std::string content, glm::vec2 destPoint, std::function<void()> calltoFunction, ResourceManager& resourceManager, TexParam texParams, float scale){
			init(texturePath, fontPath, fontSize, textScale, fontParams, content, destPoint, calltoFunction, resourceManager, texParams, scale);
		}

		void init(const char* texturePath, const char* fontPath, int fontSize, glm::vec2& textScale, TexParam fontParams, std::string content, glm::vec4 destRect, std::function<void()> calltoFunction, ResourceManager& resourceManager, TexParam texParams){
			m_texture = resourceManager.getTileSheet(texturePath, glm::ivec2(3, 1), texParams);
			m_buttonFont.initTTF(fontPath, fontSize, fontParams);
			m_rect.initialize(destRect.x, destRect.y, destRect.z, destRect.w, true);
			m_content = content;
			m_textScaling = textScale;
			m_calltoFunction = calltoFunction;
		}

		void init(const char* texturePath, const char* fontPath, int fontSize, glm::vec2& textScale, TexParam fontParams, std::string content, glm::vec2 destPoint, std::function<void()> calltoFunction, ResourceManager& resourceManager, TexParam texParams, float scale){
			m_texture = resourceManager.getTileSheet(texturePath, glm::ivec2(3, 1), texParams);
			m_buttonFont.initTTF(fontPath, fontSize, fontParams);
			glm::vec4 destRect = glm::vec4(destPoint.x, destPoint.y, (m_buttonFont.measure(content.c_str()).x + (button_text_X_padding * 2)) * scale, (m_buttonFont.getFontHeight() + (button_text_Y_padding * 2)) * scale);
			m_rect.initialize(destRect.x, destRect.y, destRect.z, destRect.w, true);
			m_content = content;
			m_textScaling = textScale;
			m_calltoFunction = calltoFunction;
		}

		void destroy(){
			m_buttonFont.dispose();
			m_calltoFunction = nullptr;
		}

		void draw(SpriteBatch& spriteBatch, Camera2D& camera, const bool& hasText = false){
			//TODO: Make color changable per button
			//glm::vec4 drawCoords(camera.convertScreenToWorld(glm::vec2(m_rect.x1, m_rect.y2)), m_rect.width, m_rect.height);
			glm::vec4 drawCoords(m_rect.x1, m_rect.y2, m_rect.width, m_rect.height);
			spriteBatch.draw(glm::vec4(drawCoords.x, drawCoords.y, drawCoords.z, drawCoords.w), m_texture.getUVs(m_currentTexture), m_texture.texture.id, ALWAYS_ON_TOP, ColorRGBA8(255, 255, 255, 255));
			if (hasText){
				m_buttonFont.draw(spriteBatch, m_content.c_str(), glm::vec2(drawCoords.x + (drawCoords.z / 2) + m_textOffset, drawCoords.y + ((drawCoords.w / 2) - ((m_buttonFont.getFontHeight() / 2) * m_textScaling.y)) - m_textOffset - button_text_Y_padding / 2), m_textScaling, ALWAYS_ON_TOP, ColorRGBA8(255, 255, 255, 255), Justification::MIDDLE);
			}
		}

		bool update(InputManager &inputManager, Camera2D& camera){
			glm::vec2 mouseCoords = camera.convertScreenToWorld(inputManager.getMouseCoords());
			m_currentTexture = 0;
			if (mouseCoords.x <= m_rect.x2 && mouseCoords.x >= m_rect.x1 && 
				mouseCoords.y <= m_rect.y1 && mouseCoords.y >= m_rect.y2){
				m_currentTexture = 1;
				if (inputManager.wasKeyDown(MouseId::BUTTON_LEFT) && !inputManager.isKeyDown(MouseId::BUTTON_LEFT)){
					m_currentTexture = 1;
					m_textOffset = 0.0f;
					if (m_calltoFunction != nullptr){
						m_calltoFunction();
						m_pressed = false;
						return true;
					}
				}
				if (inputManager.isKeyDown(MouseId::BUTTON_LEFT)){
					m_currentTexture = 2;
					m_textOffset = button_pressed_text_offset;
					m_pressed = true;
					return false;
				}
			}
			m_textOffset = 0.0f;
			m_pressed = false;
			return false;
		}

		bool Pressed(){ return m_pressed; }

	private:
		bool m_pressed = false;

		std::function<void()> m_calltoFunction = nullptr;

		std::string m_content;

		TileSheet m_texture;

		SpriteFont m_buttonFont;

		float m_textOffset = 0.0f;
		glm::vec2 m_textScaling = glm::vec2(1.0f);

		unsigned int m_currentTexture = 0;

		Rect m_rect;
	};

	//////////////////////////////////////////////////////////////////////////

	class GUIInterface
	{
	public:
		virtual ~GUIInterface(){ /* Empty */ }

		virtual void destroyComponents() = 0;

		void destroy(){
			m_GUISpritefont.dispose();
			m_GUISpritebatch.dispose();
			destroyComponents();
		}

		void init(IScreen* parentScreen, GLSLProgram* textureProgram, ResourceManager* resourceManger, glm::ivec2 dims, glm::vec2 positionOffset, float scale){
			m_parentScreen = parentScreen;
			m_textureProgram = textureProgram;
			m_resourceManager = resourceManger;
			m_GUISpritebatch.init();
			m_GUICamera.init(dims.x, dims.y);
			m_GUICamera.setScale(scale);
			m_GUICamera.setPosition(positionOffset);
			m_GUICameraBounds = glm::vec2(dims.x / scale, dims.y / scale);
			initComponents();
		}
	
		virtual void update(InputManager& inputManager) = 0;

		/* DO NOT CALL THESE FUNCTIONS EXPLICITLY */
		virtual void IDraw(float fps) = 0;
		virtual void initComponents() = 0;

		void draw(std::string textureUniform, float fps){
			glm::mat4 projectionMatrix = m_GUICamera.getCameraMatrix();
			GLint pUniform = m_textureProgram->getUniformLocation(textureUniform);
			glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

			m_GUISpritebatch.begin();

			IDraw(fps);

			m_GUISpritebatch.end();
			m_GUISpritebatch.renderBatch();
		}

		GUIButton createButton(const std::string& texturePath, const std::string& fontPath, const int& fontSize, glm::vec2& textScale, const TexParam& fontParams, const std::string& content, glm::vec4 destRect, std::function<void()> calltoFunction, const TexParam& texParams){
			GUIButton newButton;
			newButton.init(texturePath.c_str(), fontPath.c_str(), fontSize, textScale, fontParams, content, destRect, calltoFunction, *m_resourceManager, texParams);
			return newButton;
		}

		GUIButton createButton(const std::string& texturePath, const std::string& fontPath, const int& fontSize, glm::vec2& textScale, const TexParam& fontParams, const std::string& content, glm::vec2 destPoint, std::function<void()> calltoFunction, const TexParam& texParams, float scale = 1.0f){
			GUIButton newButton;
			newButton.init(texturePath.c_str(), fontPath.c_str(), fontSize, textScale, fontParams, content, destPoint, calltoFunction, *m_resourceManager, texParams, scale);
			return newButton;
		}

	protected:
		glm::vec2 m_GUICameraBounds;

		IScreen* m_parentScreen = nullptr;

		std::vector<GLTexture> m_GUIElements;
		SpriteBatch m_GUISpritebatch;
		//? Add multiple fonts possible
		SpriteFont m_GUISpritefont;
		GLSLProgram* m_textureProgram = nullptr;
		ResourceManager* m_resourceManager = nullptr;

		Camera2D m_GUICamera;
	};
	
}