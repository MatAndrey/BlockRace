#include "Menu.hpp"

Menu::Menu(sf::RenderWindow* window, float height) :
	saveButton(sf::Vector2f{ 10, 5 }, window, L"Сохранить", std::bind(&Menu::onSave, this), sf::Vector2f{ 150, height - 10 }),
	saveAsButton(sf::Vector2f{ 170, 5 }, window, L"Сохранить как", std::bind(&Menu::onSaveAs, this), sf::Vector2f{ 150, height - 10 }),
	loadButton(sf::Vector2f{ 330, 5 }, window, L"Загрузить", std::bind(&Menu::onLoad, this), sf::Vector2f{ 150, height - 10 }),
	exitButton(sf::Vector2f{ 490, 5 }, window, L"Выйти", std::bind(&Menu::onExit, this), sf::Vector2f{ 150, height - 10 }),
	height(height),
	window(window)
{}

void Menu::render()
{
	saveButton.render();
	saveAsButton.render();
	loadButton.render();
	exitButton.render();
}

void Menu::onSave()
{
	EventBus::get().publish<SaveFileEvent>(SaveFileEvent{ L"save.dat"});
}

void Menu::onSaveAs()
{
	std::wstring fileName = ShowSaveFileDialog(window->getNativeHandle());
	EventBus::get().publish<SaveFileEvent>(SaveFileEvent{ fileName });
}

void Menu::onLoad()
{
	std::wstring fileName = ShowOpenFileDialog(window->getNativeHandle());
	EventBus::get().publish<LoadFileEvent>(LoadFileEvent{ fileName });
}

void Menu::onExit()
{
	EventBus::get().publish<ExitEvent>(ExitEvent{});
}