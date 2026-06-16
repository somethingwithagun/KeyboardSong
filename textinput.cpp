#include "textinput.h"

TextInput::TextInput() : TextInput(DEFAULT_BACKGROUND_SIZE, m_font, 16)
{
	//sf::Font font = new sf:
}

TextInput::TextInput(const sf::Vector2f& bgsize, sf::Font& font, const int character_size) : m_font(font), m_label(m_font), m_placeholderLabel(m_font)
{
	m_background.setSize(bgsize);
	m_background.setFillColor(sf::Color::White);
	m_background.setOrigin(sf::Vector2f(m_background.getSize().x / 2, m_background.getSize().y / 2));

	m_label.setCharacterSize(character_size);
	m_label.setFillColor(sf::Color::Black);
	m_label.setFont(m_font);

	m_placeholderLabel.setCharacterSize(character_size);
	m_placeholderLabel.setFont(m_font);
	m_placeholderLabel.setFillColor(sf::Color(m_label.getFillColor().r, m_label.getFillColor().g, m_label.getFillColor().b, m_label.getFillColor().a + 40));
	m_placeholderLabel.setStyle(sf::Text::Italic);
}

TextInput::~TextInput() {
}

const sf::String& TextInput::getPlaceholderText() const
{
	return m_placeholderText;
}

void TextInput::update(double deltaT)
{

	switch (m_alignment)
	{
	case TextInput::Left:
		m_label.setOrigin(sf::Vector2f((m_background.getSize().x / 2) - 5, (m_background.getSize().y / 2) - m_label.getGlobalBounds().size.y / 4));
		m_placeholderLabel.setOrigin(sf::Vector2f((m_background.getSize().x / 2) - 5, (m_background.getSize().y / 2) - m_placeholderLabel.getGlobalBounds().size.y / 4));
		break;
	case TextInput::Center:
		m_label.setOrigin(sf::Vector2f(m_label.getGlobalBounds().size.x / 2, (m_label.getGlobalBounds().size.y / 2) + m_label.getGlobalBounds().size.y / 4));
		m_placeholderLabel.setOrigin(sf::Vector2f(m_placeholderLabel.getGlobalBounds().size.x / 2, (m_placeholderLabel.getGlobalBounds().size.y / 2) + m_label.getGlobalBounds().size.y / 4));
		break;
	case TextInput::Right:
		m_label.setOrigin(sf::Vector2f((m_label.getGlobalBounds().size.x - m_background.getSize().x / 2) + 5, (m_label.getGlobalBounds().size.y / 2) + m_label.getGlobalBounds().size.y / 4));
		m_placeholderLabel.setOrigin(sf::Vector2f((m_placeholderLabel.getGlobalBounds().size.x - m_background.getSize().x / 2) + 5, (m_placeholderLabel.getGlobalBounds().size.y / 2) + m_label.getGlobalBounds().size.y / 4));
		break;
	default:
		break;
	}

	if (m_text.getSize() > 0 && !m_active && m_text[m_text.getSize() - 1] == '_') {
		m_text.erase(m_text.getSize() - 1, 1);
		this->setText(m_text);
	}
	else if (m_text.getSize() > 0 && m_active && m_text[m_text.getSize() - 1] != '_')
		this->setText(m_text);

	if (m_counter > 0.06)
	{

	}

	m_counter += deltaT;
}

void TextInput::render(sf::RenderTarget* target)
{
	target->draw(m_background);

	if (m_text.getSize())
		target->draw(m_label);
	else if (m_placeholderText.getSize())
		target->draw(m_placeholderLabel);
}

void TextInput::setPlaceholderText(const sf::String& text) {
	if (text.getSize() > m_charactersLimit)
	{
		std::cerr << "Max characters limit reached!\n";
		return;
	}

	m_placeholderText = text;

	m_placeholderLabel.setString(m_placeholderText);
}

void TextInput::setAlign(TextAlign align)
{
	m_alignment = align;
}

void TextInput::setActive(bool active)
{
	m_active = active;
}

void TextInput::setFont(const sf::Font& font)
{
	m_font = font;
	m_label.setFont(m_font);
	m_placeholderLabel.setFont(m_font);
}

void TextInput::setBackgroundColor(const sf::Color& color)
{
	m_background.setFillColor(color);
}

void TextInput::setTextColor(const sf::Color& color)
{
	m_label.setFillColor(color);
	m_placeholderLabel.setFillColor(sf::Color(m_label.getFillColor().r, m_label.getFillColor().g, m_label.getFillColor().b, m_label.getFillColor().a + 40));
}

void TextInput::setCharacterSize(const unsigned int& size)
{
	m_label.setCharacterSize(size);
	m_placeholderLabel.setCharacterSize(size);
}

void TextInput::setSize(const sf::Vector2f& size)
{
	m_background.setSize(size);
}

void TextInput::setPosition(const sf::Vector2f& pos)
{
	m_background.setPosition(pos);
	m_label.setPosition(m_background.getPosition());
	m_placeholderLabel.setPosition(m_background.getPosition());
}

void TextInput::setText(const sf::String& text)
{
	if (text.getSize() > m_charactersLimit)
	{
		std::cerr << "Max characters limit reached!\n";
		return;
	}

	if (m_active)
		m_text = text + '_';

	m_label.setString(m_text);
}

void TextInput::setCharactersLimit(const unsigned int limit)
{
	m_charactersLimit = limit;
}

const int TextInput::getCharactersLimit() const
{
	return m_charactersLimit;
}

const sf::FloatRect TextInput::getGlobalBounds() const
{
	return m_background.getGlobalBounds();
}

const sf::Vector2f TextInput::getSize() const
{
	return m_background.getSize();
}

const int TextInput::getCharacterSize() const
{
	return m_label.getCharacterSize();
}

const sf::String TextInput::getText() const
{
	if (m_text.getSize() > 0 && m_active && m_text[m_text.getSize() - 1] == '_') {
		sf::String text = m_text;
		text.erase(text.getSize() - 1, 1);
		return text;
	}

	return m_text;
}

const sf::Color& TextInput::getTextColor() const
{
	return m_label.getFillColor();
}

const sf::Color& TextInput::getBackgroundColor() const
{
	return m_background.getFillColor();
}

const bool TextInput::getActive() const
{
	return m_active;
}

const TextInput::TextAlign& TextInput::getAlign() const
{
	return m_alignment;
}
