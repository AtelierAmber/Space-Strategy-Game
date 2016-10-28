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

#include <fstream>

enum DisplayState{
	MAIN,
	SAVE_NAME
};

struct ScoreStruct{
	int ranking = -1;
	char name[3];
	int score = -1;
};

class GameOver :
	public Sakura::IScreen
{
public:
	GameOver(Sakura::Window* window, int* score);
	~GameOver();

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

	void sortScores(int left, int right);
	int partition(int left, int right);
	void saveScore();

private:
	Sakura::GUIButton m_saveScore;
	Sakura::GUIButton m_retryButton;
	Sakura::GUIButton m_quitButton;
	Sakura::GUIButton m_continueButton;
	Sakura::GLTexture m_nameArrow;
	Sakura::SoundEffect m_blip;

	std::fstream m_scoreFile;

	int m_nameCharIndex = 0;
	int m_letterIndex = 0;
	std::string m_chosenNameChar[3];

	Sakura::SpriteFont m_font;

	ScoreStruct m_currentScore;
	std::vector<ScoreStruct> m_scores;

	DisplayState m_displayState = MAIN;

	Sakura::Window* m_window = nullptr;
	int* m_finalScore;
};

