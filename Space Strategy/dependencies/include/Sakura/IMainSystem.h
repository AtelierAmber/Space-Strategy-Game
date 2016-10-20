#pragma once

#include "Window.h"
#include "InputManager.h"

#include <memory>

namespace Sakura{

	class ScreenList;
	class IScreen;

	struct wDims{
		wDims(unsigned int w = 0, unsigned int h = 0) : width(w), height(h){/* Empty */ }
		unsigned int width;
		unsigned int height;
	};

	class IMainSystem {
	public:
		IMainSystem(std::string title = "DEFAULT TITLE", unsigned int screenWidth = 500, unsigned int screenHeight = 500, unsigned int windowFlags = 0, int winPositionX = -1, int winPositionY = -1);
		virtual ~IMainSystem();

		// Runs and initializes the game
		int run();
		// Exits the game
		void exitGame();

		// Called on initialization
		virtual void onInit() = 0;
		// For adding all screens
		virtual void addScreens() = 0;
		// Called when exiting
		virtual void onExit() = 0;

		void onSDLEvent(SDL_Event& evnt);

		const float getFps() const {
			return m_fps;
		}

		InputManager inputManager;

	protected:
		// Custom update function
		virtual void update(float deltaTime);
		// Custom render function
		virtual void draw();

		bool init();

		std::string m_windowTitle;
		unsigned int m_windowFlags;
		wDims m_windowDims;
		glm::ivec2 m_windowPos;

		std::unique_ptr<ScreenList> m_screenList = nullptr;
		IScreen* m_currentScreen = nullptr;
		bool m_isRunning = false;
		float m_fps = 0.0f;
		Window m_window;
	};

}