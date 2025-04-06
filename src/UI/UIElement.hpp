#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_set>
#include "../Entity.hpp"

class UIElement : public Entity {
protected:
    static inline std::unordered_set<UIElement*> allElements;
    bool isHovered = false;
    sf::View* view = nullptr;
public:
    UIElement(sf::Vector2f pos, sf::RenderWindow* win);

    virtual ~UIElement();

    virtual bool contains(const sf::Vector2f& point) const = 0;

    void updateHoverState(const sf::Vector2i& mousePos);
    static void updateCursor(sf::RenderWindow& window);
    static std::unordered_set<UIElement*> getAllElements();
    void setView(sf::View* view);
};