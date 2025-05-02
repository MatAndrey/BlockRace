#include "Menu.hpp"

Menu::Menu(sf::RenderWindow* window, float height, PopupWindow* popup) :
	saveButton(sf::Vector2f{ 10, 5 }, window, L"Сохранить", std::bind(&Menu::onSave, this), sf::Vector2f{ 150, height - 10 }),
	saveAsButton(sf::Vector2f{ 170, 5 }, window, L"Сохранить как", std::bind(&Menu::onSaveAs, this), sf::Vector2f{ 150, height - 10 }),
	loadButton(sf::Vector2f{ 330, 5 }, window, L"Загрузить", std::bind(&Menu::onLoad, this), sf::Vector2f{ 150, height - 10 }),
	levelSelectButton(sf::Vector2f{ 490, 5 }, window, L"Уровни", std::bind(&Menu::onLevelSelect, this), sf::Vector2f{ 150, height - 10 }),
	exitButton(sf::Vector2f{ 1210, 5 }, window, L"Выйти", std::bind(&Menu::onExit, this), sf::Vector2f{ 150, height - 10 }),
	height(height),
	window(window),
	popup(popup)
{}

void Menu::render()
{
	saveButton.render();
	saveAsButton.render();
	loadButton.render();
	levelSelectButton.render();
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

void Menu::onLevelSelect() {
	std::vector<std::wstring> levels = {
		L"1 уровень", L"2 уровень", L"3 уровень",
		L"4 уровень", L"5 уровень", L"6 уровень",
		L"7 уровень", L"8 уровень", L"9 уровень", L"10 уровень"
	};

	auto callback = [this](int selectedIndex) {
		std::wstring levelFile = L"./assets/maps/level" + std::to_wstring(selectedIndex + 1) + L".json";
		};

	popup->showLevelSelection(levels, callback);
}

void Menu::onExit()
{
	EventBus::get().publish<ExitEvent>(ExitEvent{});
}