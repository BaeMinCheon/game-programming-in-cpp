#include "MainMenu.h"
#include "Game.h"
#include "Renderer.h"
#include "DialogBox.h"

MainMenu::MainMenu(class Game* _pG)
	: UIScreen(_pG)
{
	mGame->SetState(Game::EMainMenu);
	SetRelativeMouseMode(false);
	SetTitle("MainMenu");
	AddButton("StartButton", [this]()
	{
		SetRelativeMouseMode(true);
		mGame->SetState(Game::EGameplay);
		Close();
	});
	AddButton("QuitButton", [this]()
	{
		new DialogBox(mGame, "QuitText", [this]()
		{
			mGame->SetState(Game::EQuit);
			Close();
		});
	});
}

MainMenu::~MainMenu()
{
	;
}