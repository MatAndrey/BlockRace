#include "StartBlock.hpp"

void StartBlock::render() { 
    sf::Transform transform;
    transform.translate(pos);

    button.pos = pos + sf::Vector2f{ 5, 5 };
    
	window->draw(shape1, transform);
    window->draw(shape2, transform);
    window->draw(outline, transform);
    window->draw(text, transform);

    button.render();
}

StartBlock::StartBlock(sf::Vector2f _pos, sf::RenderWindow* window):
    Block(_pos, {120, 30}, window),
    shape1(size), shape2({ 25, 5 }),
    button(_pos, window)
{
    canBeChild = false;
    shape1.setFillColor(sf::Color(44, 122, 65));

    shape2.setFillColor(sf::Color(44, 122, 65));
    shape2.setPosition(sf::Vector2f(20, 30));

    text.setString(L"Начало");
    text.setCharacterSize(14);
    text.setPosition({ 30, 5 });


    std::vector<sf::Vector2f> vertexes{ {0, 0}, {120, 0}, {120, 30}, {45, 30}, {45, 35}, {20, 35}, {20, 30}, {0, 30}, {0, 0} };
    outline.resize(vertexes.size());
    for (int i = 0; i < vertexes.size(); i++) {
        outline[i].position = vertexes[i];
        outline[i].color = sf::Color::White;
    }

    EventBus::get().subscribe<BlockPressedEvent>(this, &StartBlock::handlePress);
    EventBus::get().subscribe<DisableBlocksEvent>(this, &StartBlock::handleDisable);
}

bool StartBlock::isMouseOver(sf::Vector2f pos)
{
    return button.isMouseOver(pos);
}

StartBlock::~StartBlock() {

}

StartBlock* StartBlock::clone()
{
    return new StartBlock(pos, window);
}

Block* StartBlock::update(Car& car) {
    if (nextBlock != nullptr) {
        return nextBlock;
    }
    else {
        return nullptr;
    }
}

std::string StartBlock::name()
{
    return "StartBlock";
}

void StartBlock::handlePress(const BlockPressedEvent& event)
{
    if (button.isMouseOver(event.worldPos)) {
        if (button.getState()) {
            button.setState(false);
            EventBus::get().publish(StopSimulationEvent{});
        }
        else {
            if (!event.isRunning) {
                button.setState(true);
                EventBus::get().publish(StartSimulationEvent{ this });
            }            
        }
    }
}

void StartBlock::handleDisable(const DisableBlocksEvent& event)
{
    button.setState(false);
}
