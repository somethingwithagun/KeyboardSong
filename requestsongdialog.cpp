#include "requestsongdialog.h"

RequestSongDialog::RequestSongDialog(const sf::String& text, const sf::Vector2f background_size,
  const sf::Vector2f buttons_size, const sf::Color buttons_color, const sf::Font& font) : DialogBase(background_size,buttons_size,buttons_color,font)
{
  m_text.setString(text);
  m_textRelPos = {m_text.getGlobalBounds().size.x/2,175};
  m_text.setPosition(m_background.getPosition()-m_textRelPos);

  sf::Vector2f textInputSize = {background_size.x - 40,50};
  m_authorInput = new TextInput(textInputSize,m_font);
  m_authorInput->setPosition(m_background.getPosition() + sf::Vector2f(background_size.x/2, background_size.y/4));
  m_authorInput->setPlaceholderText("Author");

  m_songInput = new TextInput(textInputSize,m_font);
  m_songInput->setPosition(m_background.getPosition() + sf::Vector2f(background_size.x/2, background_size.y/2));
  m_songInput->setPlaceholderText("Song");
}

RequestSongDialog::~RequestSongDialog() { 
}

sf::String RequestSongDialog::getAuthorName() {
  return m_authorInput->getText();
}

sf::String RequestSongDialog::getSongName() {
  return m_songInput->getText(); 
}

bool RequestSongDialog::isTextSymbol(char ch) const {
	return ch >= SPACE_CODE && ch < 128;
} 

TextInput* RequestSongDialog::getActiveInput() {
  if(m_authorInput->getActive()) {
    return m_authorInput;
  } else if(m_songInput->getActive()) {
    return m_songInput;
  } else return nullptr;
}

void RequestSongDialog::onTextEntered(char ch) {
  TextInput* activeInput = getActiveInput();
  if (!activeInput) return;

  activeInput->onTextEntered(ch);
}

void RequestSongDialog::update(const float& dt) {
  DialogBase::update(dt);

  m_authorInput->update(dt);
  m_songInput->update(dt);

  if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
    if(m_authorInput->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition()))){
      m_authorInput->setActive(true);
      m_songInput->setActive(false);
    }
    else if(m_songInput->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition()))) {
      m_songInput->setActive(true);
      m_authorInput->setActive(false);
    }
    
  }
}

void RequestSongDialog::render(sf::RenderTarget* target) {
  DialogBase::render(target);
  
  m_authorInput->render(target);
  m_songInput->render(target);
}

void RequestSongDialog::setPosition(const sf::Vector2f& pos) {
  DialogBase::setPosition(pos);
  sf::Vector2f background_size = m_background.getSize();
  m_authorInput->setPosition({m_background.getPosition().x, m_background.getPosition().y - 75});
  m_songInput->setPosition({m_background.getPosition().x, m_background.getPosition().y + 25});
}
