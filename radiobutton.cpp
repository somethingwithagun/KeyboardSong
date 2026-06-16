#include "radiobutton.h"

RadioButton::RadioButton(sf::Vector2f size, sf::Color bgColor)
	: m_body(size),m_check(size/2.f), m_checked(false)
{
	m_source_background_color = bgColor;
	m_source_check_color = DEFAULT_CHECK_COLOR;

	this->m_body.setFillColor(bgColor);
  this->m_check.setFillColor(m_source_check_color);

	this->m_body.setOrigin(sf::Vector2f(this->m_body.getGlobalBounds().size.x / 2, this->m_body.getGlobalBounds().size.y / 2));
	this->m_check.setOrigin(sf::Vector2f(this->m_check.getGlobalBounds().size.x / 2, this->m_check.getGlobalBounds().size.y / 2));

	this->m_check.setPosition(m_body.getPosition());
	
	m_waitTime = 0.05f;
	m_counter = 0.f;
}

RadioButton::~RadioButton() {
}
const bool& RadioButton::getEnabled() const
{
	return m_enabled;
}
const bool& RadioButton::getChecked() const {
	return m_checked;
}
const sf::FloatRect RadioButton::getGlobalBounds() const {
  return m_body.getGlobalBounds();
}

void RadioButton::update(double deltaT) {
	m_counter += deltaT;
}

void RadioButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_body);
  if(m_checked)
	  target.draw(m_check);
}

bool RadioButton::isReady() const {
	return (m_counter >= m_waitTime);
}

void RadioButton::setChecked(bool checked) {
	m_checked = checked;
}

void RadioButton::toggle() {
  m_checked = !m_checked;
}

void RadioButton::setFillColor(const sf::Color color, const sf::Color textColor)
{
	this->m_body.setFillColor(color);
	this->m_check.setFillColor(textColor);

	m_source_background_color = color;
	m_source_check_color = textColor;
}

void RadioButton::setPosition(sf::Vector2f pos)
{
	this->m_body.setPosition(pos);
	this->m_check.setPosition(this->m_body.getPosition());
}

void RadioButton::setEnabled(bool enabled)
{
	m_enabled = enabled;

	if (m_enabled)
	{
		m_body.setFillColor(m_source_background_color);
		m_check.setFillColor(m_source_check_color);
	}
	else
	{
		m_body.setFillColor(sf::Color(m_source_background_color.r, m_source_background_color.g, m_source_background_color.b, m_source_background_color.a - 80));
		m_check.setFillColor(sf::Color(m_source_check_color.r, m_source_check_color.g, m_source_check_color.b, m_source_check_color.a - 80));
	}
}

void RadioButton::setWaitTime(const float& waitTime) {
	m_waitTime = waitTime;
	m_counter = 0.f;
}

const sf::Vector2f RadioButton::getSize() const
{
	return m_body.getSize();
}

const sf::Vector2f RadioButton::getPosition() const
{
	return m_body.getPosition();
}

const sf::Color& RadioButton::getButtonColor() const
{
	return this->m_body.getFillColor();
}
