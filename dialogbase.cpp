#include "dialogbase.h"

DialogBase::DialogBase(const sf::Vector2f background_size, const sf::Vector2f buttons_size, const sf::Color buttons_color,const sf::Font& font, Buttons mode)
	: m_buttonsSize(buttons_size), m_font(font), m_text(m_font)
{
	m_is_opened = false;
	m_result = Result::None;


	m_background.setSize(background_size);
	m_background.setOrigin(sf::Vector2f(m_background.getSize().x / 2, m_background.getSize().y / 2));
	m_background.setFillColor(sf::Color(130, 128, 124));

	m_text.setString("");
	m_text.setFont(m_font);
	m_textRelPos = {(m_text.getGlobalBounds().size.x / 2),50};
	m_text.setPosition(m_background.getPosition()-m_textRelPos);

	sf::Vector2f reject_btn_pos(m_background.getPosition().x + (m_background.getSize().x / 2) - (buttons_size.x / 2) - 40, m_background.getPosition().y + (m_background.getSize().y / 2) - 40);
	sf::Vector2f accept_btn_pos(m_background.getPosition().x - (m_background.getSize().x / 2) + (buttons_size.x / 2) + 40, m_background.getPosition().y + (m_background.getSize().y / 2) - 40);
	sf::Vector2f close_btn_pos(m_background.getPosition().x + (m_background.getSize().x / 2) - (50 / 2), m_background.getPosition().y - (m_background.getSize().y / 2) + (50 / 2));
	//sf::Vector2f accept_btn_pos(m_background.getPosition().x + (m_background.getSize().x / 2) - (m_accept_button->getSize().x / 2) - 40, m_background.getPosition().y);

	switch (mode)
	{
	case DialogBase::Accept:
		m_acceptButton = new Button(buttons_size, "Ok", m_font, buttons_color);
		m_acceptButton->setPosition(accept_btn_pos);
		break;
	case DialogBase::Reject:
		m_rejectButton = new Button(buttons_size, "Cancel", m_font, buttons_color);
		m_rejectButton->setPosition(reject_btn_pos);
		break;
	case DialogBase::Close:
		m_closeButton = new Button(sf::Vector2f(50, 50), "X", m_font, RED);
		m_closeButton->setPosition(close_btn_pos);
		break;
	case DialogBase::Accept | DialogBase::Reject | DialogBase::Close:
		m_acceptButton = new Button(buttons_size, "Ok", m_font, buttons_color);
		m_acceptButton->setPosition(accept_btn_pos);

		m_rejectButton = new Button(buttons_size, "Cancel", m_font, buttons_color);
		m_rejectButton->setPosition(reject_btn_pos);

		m_closeButton = new Button(sf::Vector2f(50, 50), "X", m_font, RED);
		m_closeButton->setPosition(close_btn_pos);
		break;

	case DialogBase::Accept | DialogBase::Reject:
		m_acceptButton = new Button(buttons_size, "Ok", m_font, buttons_color);
		m_acceptButton->setPosition(accept_btn_pos);

		m_rejectButton = new Button(buttons_size, "Cancel", m_font, buttons_color);
		m_rejectButton->setPosition(reject_btn_pos);
		break;
	default:
		break;
	}
}

// DialogBase::DialogBase()
// {
// }

DialogBase::~DialogBase()
{
	if (m_acceptButton)
		delete m_acceptButton;
	if (m_rejectButton)
		delete m_rejectButton;
	if (m_closeButton)
		delete m_closeButton;

	if (m_backgroundTexture)
		delete m_backgroundTexture;
}

void DialogBase::update(const float& dt)
{
	m_text.getGlobalBounds().size.x / 2, (m_text.getGlobalBounds().size.y / 2) + m_text.getGlobalBounds().size.y / 4;

	if (m_acceptButton)
		m_acceptButton->update(dt);

	if (m_closeButton)
		m_closeButton->update(dt);

	if (m_rejectButton)
		m_rejectButton->update(dt);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		sf::Vector2f mouse_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition());

		if (m_closeButton && m_closeButton->getGlobalBounds().contains(sf::Vector2f(mouse_pos.x, mouse_pos.y)))
		{
			if (m_counterA >= 0.006)
			{
				m_closeButton->setPressed(true);
				m_counterA = 0;

				m_is_opened = false;

				m_result = Result::Closed;
			}
		}
		else if (m_acceptButton && m_acceptButton->getGlobalBounds().contains(sf::Vector2f(mouse_pos.x, mouse_pos.y)))
		{
			if (m_counterB >= 0.006)
			{
				m_acceptButton->setPressed(true);
				m_counterB = 0;

				m_is_opened = false;

				m_result = Result::Accepted;
			}
		}
		else if (m_rejectButton && m_rejectButton->getGlobalBounds().contains(sf::Vector2f(mouse_pos.x, mouse_pos.y)))
		{
			if (m_counterC >= 0.006)
			{
				m_rejectButton->setPressed(true);
				m_counterC = 0;

				m_is_opened = false;

				m_result = Result::Rejected;
			}
		}
	}
	m_counterA += dt;
	m_counterB += dt;
	m_counterC += dt;
}

void DialogBase::render(sf::RenderTarget* target)
{

	if (m_is_opened)
	{
		target->draw(m_background);

		if (m_acceptButton)
			target->draw(*m_acceptButton);

		if (m_closeButton)
			target->draw(*m_closeButton);

		if (m_rejectButton)
			target->draw(*m_rejectButton);

		target->draw(m_text);
	}
}

void DialogBase::show()
{
	m_is_opened = true;
}

void DialogBase::close()
{
	m_is_opened = false;
}

void DialogBase::setText(const sf::String& text)
{
	m_text.setString(text);
	m_text.setPosition(sf::Vector2f(m_background.getPosition().x - (m_text.getGlobalBounds().size.x / 2), m_background.getPosition().y - 50));
}

void DialogBase::setPosition(const sf::Vector2f& pos)
{
	m_background.setPosition(pos);

	m_text.setPosition(m_background.getPosition()-m_textRelPos);

	if (m_acceptButton)
	{
		sf::Vector2f accept_btn_pos(m_background.getPosition().x - (m_background.getSize().x / 2) + (m_buttonsSize.x / 2) + 40, m_background.getPosition().y + (m_background.getSize().y / 2) - 40);
		m_acceptButton->setPosition(accept_btn_pos);
	}

	if (m_rejectButton)
	{
		sf::Vector2f reject_btn_pos(m_background.getPosition().x + (m_background.getSize().x / 2) - (m_buttonsSize.x / 2) - 40, m_background.getPosition().y + (m_background.getSize().y / 2) - 40);
		m_rejectButton->setPosition(reject_btn_pos);
	}

	if (m_closeButton)
	{
		sf::Vector2f close_btn_pos(m_background.getPosition().x + (m_background.getSize().x / 2) - (50 / 2), m_background.getPosition().y - (m_background.getSize().y / 2) + (50 / 2));
		m_closeButton->setPosition(close_btn_pos);
	}
}

void DialogBase::setFont(const sf::Font& font) {
	m_font = font;
}

void DialogBase::symEntered(char c) {

}

void DialogBase::symErase() {

}

DialogBase::Result DialogBase::getResult() const
{
	return m_result;
}

const bool& DialogBase::getIsOpen() const
{
	return m_is_opened;
}
