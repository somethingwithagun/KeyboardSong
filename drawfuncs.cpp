#include "drawfuncs.h"

sf::RectangleShape* create_line(const int x1, const int y1,
      const int x2, const int y2, const int thickness, sf::Color color) {
  float d = sqrt((x2-x1)*(x2-x1) +(y2-y1)*(y2-y1));
  sf::RectangleShape* line = new sf::RectangleShape(sf::Vector2f(d, thickness));
  line->setPosition(sf::Vector2f(x1,y1));
  line->setFillColor(color);
  float cos = (x2-x1)/d;
  int angle = round(acosf(cos) * 180.0f/ PI);
  if(y1 > y2)
    angle = -angle;
  line->setRotation(sf::degrees(angle));
  return line;
}
void edit_line(sf::RectangleShape* line, const int x1, const int y1,
        const int x2, const int y2, const int thickness, sf::Color color) {
  float d = sqrt((x2-x1)*(x2-x1) +(y2-y1)*(y2-y1));
  line->setSize(sf::Vector2f(d,thickness));
  line->setPosition(sf::Vector2f(x1,y1));
  line->setFillColor(color);
  float cos = (float)(x2-x1)/d;
  int angle = round(acosf(cos) * 180.0/ PI);
  if(y1 > y2)
    angle = -angle;
  line->setRotation(sf::degrees(angle));
}

sf::RectangleShape* create_rect(const int x, const int y,
              const int width, const int height, sf::Color color) {
  sf::RectangleShape* rect = new sf::RectangleShape(sf::Vector2f(width,height));
  rect->setFillColor(color);
  rect->setPosition(sf::Vector2f(x,y));
  return rect;
}
void edit_rect(sf::RectangleShape* rect, const int x,
  const int y, const int width, const int height, sf::Color color) {
  rect->setSize(sf::Vector2f(width,height));
  rect->setFillColor(color);
  rect->setPosition(sf::Vector2f(x,y));
}

sf::CircleShape* create_ellipse(const int x, const int y, const int width, const int height,
        sf::Color color) {
  sf::CircleShape* ellipse = new sf::CircleShape(width);
  ellipse->setPosition(sf::Vector2f(x,y));
  ellipse->setFillColor(color);
  return ellipse;
}
void edit_ellipse(sf::CircleShape* circle, const int x,
                                 const int y, const int width,
                                 const int height, sf::Color color) {
  circle->setRadius(width);
  circle->setPosition(sf::Vector2f(x,y));
  circle->setFillColor(color);
}

sf::Text* create_text(const sf::String& string, const int x, const int y,
        const int character_size, sf::Font* font, sf::Color color) {
  sf::Text* text = new sf::Text(*font);
  text->setString(string);
  text->setCharacterSize(character_size); 
  text->setPosition(sf::Vector2f(x,y));
  text->setFillColor(color);
  sf::FloatRect bounds = text->getGlobalBounds();
  text->setOrigin(sf::Vector2f(bounds.size.x/2,bounds.size.y/2 + 10));
  return text;
}
void edit_text(sf::Text* text, const sf::String& string,
                              const int x, const int y,
                              const int character_size, sf::Font* font, sf::Color color) {
  text->setString(string);
  text->setCharacterSize(character_size); 
  text->setFont(*font);
  text->setPosition(sf::Vector2f(x,y));
  text->setFillColor(color);
  sf::FloatRect bounds = text->getGlobalBounds();
  text->setOrigin(sf::Vector2f(bounds.size.x/2,bounds.size.y/2 + 10));
}
