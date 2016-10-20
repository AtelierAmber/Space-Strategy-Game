#pragma once

#include <vector>

namespace Sakura{

	class IMainSystem;
	class IScreen;

	class ScreenList
	{
	public:
		ScreenList(IMainSystem* game);
		~ScreenList();

		IScreen* moveNext();
		IScreen* movePrevious();

		void setScreen(int nextScreen);
		void addScreen(IScreen* newScreen);

		void destroy();

		IScreen* getCurrent();

	protected:
		IMainSystem* m_game = nullptr;
		std::vector<IScreen*> m_screens;
		int m_currentScreenIndex = -1;
	};

}