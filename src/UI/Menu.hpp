#pragma once 
#include "Button.hpp"
#include "../FileDialog.hpp"
#include "PopupWindow.hpp"

class Menu {
	sf::RenderWindow* window;
	PopupWindow* popup;
	int availableLevelsCount = 1;

	Button saveButton;
	Button saveAsButton;
	Button loadButton;
	Button clearBlocksButton;
	Button levelSelectButton;
	Button credits;
	Button exitButton;

	void onSave();
	void onSaveAs();
	void onLoad();
	void onClear();
	void onLevelSelect();
	void onCredits();
	void onExit();

	void onUnlockLevelsCheat(const CheatUnlockLevelsEvent&);
public:
	float height;
	Menu(sf::RenderWindow* window, float height, PopupWindow* popup);
	void setAvailableLevelsCount(int count);
	void render();
};