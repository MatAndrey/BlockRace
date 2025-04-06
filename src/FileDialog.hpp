#pragma once
#include <windows.h>
#include <commdlg.h>
#include <string>
#include <filesystem>
#include <SFML/Graphics.hpp>

std::wstring ShowSaveFileDialog(sf::WindowHandle parentHandle);

std::wstring ShowOpenFileDialog(sf::WindowHandle parentHandle);