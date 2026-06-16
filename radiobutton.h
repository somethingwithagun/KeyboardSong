#ifndef RADIOBUTTON_H
#define RADIOBUTTON_H

#define DEFAULT_CHECK_COLOR sf::Color::Green
#define DEFAULT_SIZE sf::Vector2f(25,25)

#include <SFML/Graphics.hpp>
#include "constants.h"

class RadioButton : public sf::Drawable
{
public:
	RadioButton(sf::Vector2f size, sf::Color bgColor = sf::Color::Red);

	~RadioButton();

	void update(double deltaT);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	bool isReady() const;

	// setters
	void setChecked(bool checked);
	void toggle();

	void setFillColor(sf::Color color, sf::Color textColor = sf::Color::White);

	void setPosition(sf::Vector2f pos);

	void setEnabled(bool enabled);

	void setWaitTime(const float& waitTime);

	// getters
	const bool& getEnabled() const;
	const bool& getChecked() const;

	const sf::FloatRect getGlobalBounds() const;

	const sf::Vector2f getSize() const;

	const sf::Vector2f getPosition() const;

	const sf::Color& getButtonColor() const;

	const sf::Color& getCheckColor() const;

private:
	sf::Color m_source_background_color;
	sf::Color m_source_check_color;

	float m_waitTime;
	float m_counter;

	bool m_enabled;
	bool m_checked;
		
	sf::RectangleShape m_body;
	sf::RectangleShape m_check;
};
#endif