#pragma once 
#include "Button.hpp"
#include "../FileDialog.hpp"

class Menu {
	sf::RenderWindow* window;

	Button saveButton;
	Button saveAsButton;
	Button loadButton;
	Button exitButton;

	void onSave();
	void onSaveAs();
	void onLoad();
	void onExit();
public:
	float height;
	Menu(sf::RenderWindow* window, float height);
	void render();
};