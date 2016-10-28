#include "BaseGame.h"

#ifdef _DEBUG
#include <vld.h>
#endif

#define flags Sakura::WindowFlags::BORDERLESS

int main(int argc, char** argv){
	BaseGame game("Celistia Defense", (int)(1000), (int)(750), flags, -100, -100);
	return game.run();
}