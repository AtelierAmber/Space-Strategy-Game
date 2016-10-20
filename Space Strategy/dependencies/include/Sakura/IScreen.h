#pragma once
#include "SpriteBatch.h"
#include "DebugRenderer.h"
#include "GLSLProgram.h"
#include "Camera2D.h"
#include "ResourceManager.h"

#define SCREEN_INDEX_NO_SCREEN -1

/* Upload texture uniform via a macro for readability */
#define register_texture_uniform(Tprogram, uniform) GLint textureUniform = Tprogram.getUniformLocation(uniform);\
													glUniform1i(textureUniform, 0);\
													glActiveTexture(GL_TEXTURE0);

#define camera_matrix(camera) camera.getCameraMatrix()

/* Camera matrix via a macro for readability */
#define register_camera_matrix(camera, Tprogram, uniform) glm::mat4 projectionMatrix = camera_matrix(camera);\
								  GLint pUniform = Tprogram.getUniformLocation(uniform);\
								  glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

namespace Sakura{

	class IMainSystem;

	enum class ScreenState{
		NONE,
		RUNNING,
		EXIT_APPLICATION,
		CHANGE_NEXT,
		CHANGE_PREVIOUS
	};

	class IScreen
	{
	public:
		friend class ScreenList;
		IScreen(){
			//Empty
		}

		virtual ~IScreen(){
			//Empty
		}

		//Returns the index of the next or previous screen when changing screens
		virtual int getNextScreenIndex() const = 0;
		virtual int getPreviousScreenIndex() const = 0;

		//Called at beginning and end of application
		virtual void build() = 0;
		virtual void destroy() = 0;

		//Called when a screen enters and exits focus
		virtual void onEntry() = 0;
		virtual void onExit() = 0;

		void EngineDrawMeathod(){
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_textureProgram.use();

			register_texture_uniform(m_textureProgram, m_GLSLSamplerName);
			/* GLint textureUniform = Tprogram.getUniformLocation(uniform); 
			* glUniform1i(textureUniform, 0); 
			* glActiveTexture(GL_TEXTURE0); */

			register_camera_matrix(m_camera, m_textureProgram, m_GLSLPUniformName);
			/*glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
			* GLint pUniform = m_textureProgram.getUniformLocation("P");
			* glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);*/

			m_spriteBatch.begin(m_glyphSortType);

			draw();

			m_spriteBatch.end();
			m_spriteBatch.renderBatch();

			/* For Elements that require use of a seperate 
			 * spritebatch, camera, or otherwise need a specific element*/
			specificDraw();

			m_textureProgram.unuse();

			m_debugRenderer.end();
			m_debugRenderer.render(camera_matrix(m_camera), 1.0f, m_GLSLPUniformName);
		}

		//Called in the main game loop
		virtual void update(float deltaTime) = 0;
		virtual void draw() = 0;
		virtual void specificDraw() = 0;

		int getScreenIndex() const { return m_screenIndex; }
		void setRunning(){
			m_screenState = ScreenState::RUNNING;
		}

		ScreenState getState() const { return m_screenState; }
		void setState(ScreenState screenState){ m_screenState = screenState; }

		void setParentGame(IMainSystem* game) { m_game = game; }

		int NextScreen() const { return m_nextScreen; }
		void SetNextScreen(int val) { m_nextScreen = val; }
	protected:

		std::string m_GLSLSamplerName = "";
		std::string m_GLSLPUniformName = "";
		
		Sakura::SpriteBatch m_spriteBatch;
		GlyphSortType m_glyphSortType = GlyphSortType::TEXTURE;
		Sakura::DebugRenderer m_debugRenderer;
		Sakura::GLSLProgram m_textureProgram;
		Sakura::Camera2D m_camera;
		Sakura::ResourceManager m_resourceManager;

		ScreenState m_screenState = ScreenState::NONE;
		IMainSystem* m_game = nullptr;

		int m_nextScreen = SCREEN_INDEX_NO_SCREEN;

		int m_screenIndex = -1;
		
	};

}