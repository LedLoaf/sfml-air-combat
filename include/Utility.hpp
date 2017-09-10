#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>

#include <string>

namespace sf
{
	class Sprite;
	class Text;
}

class Animation;

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);
void centerOrigin(Animation& animation);

float toDegree(float radian);
float toRadian(float degree);

float length(sf::Vector2f vector);
sf::Vector2f unitVector(sf::Vector2f vector);

int randomInt(int exclusiveMax);

std::string toString(sf::Keyboard::Key key);

#endif // UTILITY_HPP
