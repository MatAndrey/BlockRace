#include "UIElement.hpp"

UIElement::UIElement(sf::Vector2f pos, sf::RenderWindow* win): Entity(pos, win) {
    allElements.insert(this);
}

UIElement::~UIElement()
{
    allElements.erase(this);
}

void UIElement::updateHoverState(const sf::Vector2i& mousePos)
{
    sf::Vector2f localPos = view ? window->mapPixelToCoords(mousePos, *view) : window->mapPixelToCoords(mousePos);
    bool nowHovered = contains(localPos);
    if (nowHovered != isHovered) {
        isHovered = nowHovered;
    }
}

void UIElement::updateCursor(sf::RenderWindow& window)
{
    static sf::Cursor handCursor = sf::Cursor::createFromSystem(sf::Cursor::Type::Hand).value(), 
        arrowCursor = sf::Cursor::createFromSystem(sf::Cursor::Type::Arrow).value();

    bool anyHovered = std::any_of(
        allElements.begin(),
        allElements.end(),
        [](UIElement* elem) { return elem->isHovered; }
    );

    window.setMouseCursor(anyHovered ? handCursor : arrowCursor);
}

std::unordered_set<UIElement*> UIElement::getAllElements()
{
    return allElements;
}

void UIElement::setView(sf::View* view)
{
    this->view = view;
}
