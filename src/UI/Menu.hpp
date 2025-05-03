#pragma once 
#include "Button.hpp"
#include "../FileDialog.hpp"
#include "PopupWindow.hpp"

class Menu {
	sf::RenderWindow* window;
	PopupWindow* popup;

	Button saveButton;
	Button saveAsButton;
	Button loadButton;
	Button clearBlocksButton;
	Button levelSelectButton;
	Button exitButton;

	void onSave();
	void onSaveAs();
	void onLoad();
	void onClear();
	void onLevelSelect();
	void onExit();
public:
	float height;
	Menu(sf::RenderWindow* window, float height, PopupWindow* popup);
	void render();
};