#include "BaseGame.h"

#ifdef _DEBUG
#include <vld.h>
#define flags Sakura::WindowFlags::BORDERLESS
#else
#define flags Sakura::WindowFlags::BORDERLESS | Sakura::WindowFlags::FULLSCREEN
#endif

int main(int argc, char** argv){
	BaseGame game("Game", (int)(1000), (int)(750), flags, -100, -100);
	return game.run();
}