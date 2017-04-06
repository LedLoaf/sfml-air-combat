#include "TextNode.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

TextNode::TextNode(const FontHolder& fonts, const std::string& text)
{
	mText.setFont(fonts.get(FontID::Main));
	mText.setCharacterSize(20);
	setString(text, false);
}

void TextNode::setString(const std::string& text, bool center)
{
	mText.setString(text);
	if (center == true)
	{
		centerOrigin(mText);
	}
}

void TextNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mText, states);
}

