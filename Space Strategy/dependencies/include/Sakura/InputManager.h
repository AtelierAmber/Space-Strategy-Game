#pragma once

#include <unordered_map>
#include "KeyCode.h"
#include <glm/glm.hpp>

namespace Sakura{

// Input manager stores a key map that maps Keys to booleans.
// If the value in the key map is true, then the key is pressed.
// Otherwise, it is released.
class InputManager
{
public:
	InputManager();
    ~InputManager();

	void update();

	void pressKey(KeyCode keyID);
	void releaseKey(KeyCode keyID);

    void setMouseCoords(float x, float y);

	//if key is held
	bool isKeyDown(KeyCode keyID);

	//if key is just pressed
	bool isKeyPressed(KeyCode keyID);

	//if key was released
	bool wasKeyDown(KeyCode keyID);

    //getters
    glm::vec2 getMouseCoords() const { return m_mouseCoords; }
private:
	int m_updateItter = 0;

	std::unordered_map<KeyCode, bool> m_keyMap;
	std::unordered_map<KeyCode, bool> m_previousKeyMap;
    glm::vec2 m_mouseCoords;
};

}