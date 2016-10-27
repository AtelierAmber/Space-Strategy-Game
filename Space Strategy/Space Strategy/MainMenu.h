#pragma once
#include <Sakura/IScreen.h>
#include <Sakura/SpriteBatch.h>
#include <Sakura/SpriteFont.h>
#include <Sakura/DebugRenderer.h>
#include <Sakura/GLSLProgram.h>
#include <Sakura/Camera2D.h>
#include <Sakura/ResourceManager.h>
#include <Sakura/GUIInterface.h>
#include <Sakura/Window.h>

class MenuGUI :
	public Sakura::GUIInterface
{

public:
	virtual void destroyComponents() override;


	virtual void update(Sakura::InputManager& inputManager) override;


	virtual void IDraw(float fps) override;


	virtual void initComponents() override;
	void initButtons(Sakura::Window* window);

private:

	Sakura::GUIButton m_startGame;
	Sakura::GUIButton m_quitButton;
};

class MainMenu :
	public Sakura::IScreen
{
public:
	MainMenu(Sakura::Window* window);
	~MainMenu();

	//Returns the index of the next or previous screen when changing screens
	int getNextScreenIndex() const override;
	int getPreviousScreenIndex() const override;

	//Called at beginning and end of application
	void build() override;
	void destroy() override;

	//Called when a screen enters and exits focus
	void onEntry() override;
	void onExit() override;

	//Called in the main game loop
	void update(float deltaTime) override;
	void checkInput();
	void draw() override;
	void specificDraw() override;
private:

	Sakura::Window* m_window;
	MenuGUI m_interface;
	Sakura::GLTexture m_background;

	float m_deltaTime;
};

