#include "Menu.hpp"
#include <iostream>

Menu::Menu(sf::RenderWindow* window, float height, PopupWindow* popup) :
	saveButton(sf::Vector2f{ 10, 5 }, window, L"���������", std::bind(&Menu::onSave, this), sf::Vector2f{ 150, height - 10 }),
	saveAsButton(sf::Vector2f{ 170, 5 }, window, L"��������� ���", std::bind(&Menu::onSaveAs, this), sf::Vector2f{ 150, height - 10 }),
	loadButton(sf::Vector2f{ 330, 5 }, window, L"���������", std::bind(&Menu::onLoad, this), sf::Vector2f{ 150, height - 10 }),
	clearBlocksButton(sf::Vector2f{ 490, 5 }, window, L"��������", std::bind(&Menu::onClear, this), sf::Vector2f{ 150, height - 10 }),
	levelSelectButton(sf::Vector2f{ 650, 5 }, window, L"������", std::bind(&Menu::onLevelSelect, this), sf::Vector2f{ 150, height - 10 }),
	credits(sf::Vector2f{ 810, 5 }, window, L"�����", std::bind(&Menu::onCredits, this), sf::Vector2f{ 150, height - 10 }),
	exitButton(sf::Vector2f{ 970, 5 }, window, L"�����", std::bind(&Menu::onExit, this), sf::Vector2f{ 150, height - 10 }),
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
		L"1 �������", L"2 �������", L"3 �������",
		L"4 �������", L"5 �������", L"6 �������",
		L"7 �������", L"8 �������", L"9 �������", L"10 �������"
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
	{L"������� �����������", L"������� ������"},
	{L"�������� ������������", L"����� �����"},
	{L"������������", L"������� ������", L"����� �����"},
	{L"��������", L"������� ������"},
	{L"����������� ����������", L"������� ������", L"����� �����"},
	{L"�������� �������", L"������� ������"},
	{L"������������ �������", L"������� ������"},
	{L"���������", L"����� �����"},
	{L"�����������", L"����� �����"},
	{L"�������� �����������", L"������� ������"},
	{L"���������� �� ������������", L"����� �����"},
	{L"��������", L"����� �����"},
	{L"����������� ������", L"������� ������"},
	{L"����������� ��������", L"������� ������"},
	{L"�������� ������", L"����� �����"},
	};
	popup->showCredits(credits);
}

void Menu::onExit()
{
	EventBus::get().publish<ExitEvent>(ExitEvent{});
}