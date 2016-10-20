#include "BaseGame.h"

#ifdef _DEBUG
#include <vld.h>
#endif

int main(int argc, char** argv){
	BaseGame game("Game", (int)(1280), (int)(700), 0, -100, -100);
	return game.run();
}