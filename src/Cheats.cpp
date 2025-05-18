#include "Cheats.hpp"
#include <iostream>

bool endsWith(const std::string& str, const std::string& suffix) {
	if (suffix.size() > str.size()) return false;
	return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}

void Cheats::handleTextInput(const sf::Event::KeyPressed& keyPressed)
{
	code.push_back((char)(static_cast<int>(keyPressed.code) + 97));
	if (code.size() > 100) {
		code.erase(0, code.size() - 100);
	}
	if (endsWith(code, "race")) {
		EventBus::get().publish<CheatRaceEvent>(CheatRaceEvent{});
	}
	if (endsWith(code, "showborders")) {
		EventBus::get().publish<CheatShowBordersEvent>(CheatShowBordersEvent{});
	}
	if (endsWith(code, "noborders")) {
		EventBus::get().publish<CheatNoBordersEvent>(CheatNoBordersEvent{});
	}
	if (endsWith(code, "unlock")) {
		EventBus::get().publish<CheatUnlockLevelsEvent>(CheatUnlockLevelsEvent{});
	}
}

Cheats::Cheats()
{
	EventBus::get().subscribe<sf::Event::KeyPressed >(this, &Cheats::handleTextInput);
}
