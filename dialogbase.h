#pragma once
#include <SFML/Graphics.hpp>
#include "button.h"
#include <memory>

class DialogBase
{
public:
	enum Result
	{
		None = 1 << 0,
		Accepted = 1 << 1,
		Rejected = 1 << 2,
		Closed = 1 << 3
	};
	enum Buttons
	{
		Accept = 1 << 4,
		Reject = 1 << 5,
		Close = 1 << 6
	};

protected:

	bool m_is_opened;

	Button m_acceptButton;
	Button m_rejectButton;

	Button m_closeButton;

	sf::RectangleShape m_background;

	sf::Texture* m_backgroundTexture = nullptr;
	sf::Font m_font;

	Result m_result;

	sf::Text m_text;
	sf::Vector2f m_textRelPos;


	sf::Vector2f m_buttonsSize;
private:
	float m_counterA = 0.f; // close button counter
	float m_counterB = 0.f; // accept button counter
	float m_counterC = 0.f; // reject button counter

public:
	DialogBase(const sf::Vector2f background_size, const sf::Vector2f buttons_size, const sf::Color buttons_color,const sf::Font& font, Buttons mode = static_cast<Buttons>(Buttons::Accept | Buttons::Reject | Buttons::Close));
	//DialogBase();
	~DialogBase();

	virtual void update(const float& dt);
	virtual void render(sf::RenderTarget* target);

	virtual void show();
	virtual void close();
	void setText(const sf::String& text);
	void setPosition(const sf::Vector2f& pos);
	void setFont(const sf::Font& font);

	void symEntered(char c);
  	void symErase();

	virtual Result getResult() const;
	virtual const bool& getIsOpen() const;

};

