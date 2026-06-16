#ifndef DRAWFUNCS_H_
#define DRAWFUNCS_H_

#define PI 3.14159265

#include <SFML/Graphics.hpp>
#include <cmath>
sf::RectangleShape* create_line(const int x1, const int y1,
    const int x2, const int y2,const int thickness, sf::Color color = sf::Color::Black);

void edit_line(sf::RectangleShape* line, const int x1, const int y1,
    const int x2, const int y2, const int thickness, sf::Color color = sf::Color::Black);

sf::RectangleShape* create_rect(const int x, const int y,
    const int width, const int height, sf::Color color = sf::Color::Black);

void edit_rect(sf::RectangleShape* rect,const int x, const int y,
    const int width, const int height, sf::Color color = sf::Color::Black);

sf::CircleShape* create_ellipse(const int x, const int y,
    const int width, const int height, sf::Color color = sf::Color::Black);

void edit_ellipse(sf::CircleShape* circle,const int x, const int y,
    const int width, const int height, sf::Color color = sf::Color::Black);

sf::Text* create_text(const sf::String& string, const int x, const int y,
    const int character_size, sf::Font* font, sf::Color color = sf::Color::Black);

void edit_text(sf::Text* text, const sf::String& string, const int x, const int y,
    const int character_size, sf::Font* font, sf::Color color = sf::Color::Black);



#endif