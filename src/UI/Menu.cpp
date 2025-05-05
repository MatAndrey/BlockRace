#include "Menu.hpp"
#include <iostream>

Menu::Menu(sf::RenderWindow* window, float height, PopupWindow* popup) :
	saveButton(sf::Vector2f{ 10, 5 }, window, L"Сохранить", std::bind(&Menu::onSave, this), sf::Vector2f{ 150, height - 10 }),
	saveAsButton(sf::Vector2f{ 170, 5 }, window, L"Сохранить как", std::bind(&Menu::onSaveAs, this), sf::Vector2f{ 150, height - 10 }),
	loadButton(sf::Vector2f{ 330, 5 }, window, L"Загрузить", std::bind(&Menu::onLoad, this), sf::Vector2f{ 150, height - 10 }),
	clearBlocksButton(sf::Vector2f{ 490, 5 }, window, L"Очистить", std::bind(&Menu::onClear, this), sf::Vector2f{ 150, height - 10 }),
	levelSelectButton(sf::Vector2f{ 650, 5 }, window, L"Уровни", std::bind(&Menu::onLevelSelect, this), sf::Vector2f{ 150, height - 10 }),
	credits(sf::Vector2f{ 810, 5 }, window, L"Титры", std::bind(&Menu::onCredits, this), sf::Vector2f{ 150, height - 10 }),
	exitButton(sf::Vector2f{ 970, 5 }, window, L"Выйти", std::bind(&Menu::onExit, this), sf::Vector2f{ 150, height - 10 }),
	height(height),
	window(window),
	popup(popup)
{}

void Menu::render()
{
	saveButton.render();
	saveAsButton.render();
	loadButton.render();
	clearBlocksButton.render();
	levelSelectButton.render();
	credits.render();
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

void Menu::onClear()
{
	EventBus::get().publish<ClearBlocksEvent>(ClearBlocksEvent{});
}

void Menu::onLevelSelect() {
	std::vector<std::wstring> levels = {
		L"1 уровень", L"2 уровень", L"3 уровень",
		L"4 уровень", L"5 уровень", L"6 уровень",
		L"7 уровень", L"8 уровень", L"9 уровень", L"10 уровень"
	};

	auto callback = [this](int selectedIndex) {
		std::wstring levelFile = L"./assets/maps/level" + std::to_wstring(selectedIndex + 1) + L".json";
		EventBus::get().publish<SetLevelEvent>(SetLevelEvent{ levelFile });
		};

	popup->showLevelSelection(levels, callback);
}

void Menu::onCredits()
{
	std::vector<std::vector<std::wstring>> credits = {
	{L"Ведущий программист", L"Матюшин Андрей"},
	{L"Помощник программиста", L"Чубич Артем"},
	{L"Геймдизайнер", L"Матюшин Андрей", L"Чубич Артем"},
	{L"Аниматор", L"Матюшин Андрей"},
	{L"Разработчик интерфейса", L"Матюшин Андрей", L"Чубич Артем"},
	{L"Дизайнер уровней", L"Матюшин Андрей"},
	{L"Руководитель проекта", L"Матюшин Андрей"},
	{L"Сценарист", L"Чубич Артем"},
	{L"Тестировщик", L"Чубич Артем"},
	{L"Менеджер локализации", L"Матюшин Андрей"},
	{L"Специалист по спецэффектам", L"Чубич Артем"},
	{L"Продюсер", L"Чубич Артем"},
	{L"Разработчик физики", L"Матюшин Андрей"},
	{L"Графический дизайнер", L"Матюшин Андрей"},
	{L"Редактор титров", L"Чубич Артем"},
	};
	popup->showCredits(credits);
}

void Menu::onExit()
{
	EventBus::get().publish<ExitEvent>(ExitEvent{});
}