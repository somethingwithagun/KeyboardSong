#include "loadlyricsdialog.h"

LoadLyricsDialog::LoadLyricsDialog(const sf::String& text, const sf::Vector2f background_size,
  const sf::Vector2f buttons_size, const sf::Color buttons_color, const sf::Font& font) : DialogBase(background_size,buttons_size,buttons_color,font) {
  m_text.setString(text);
  m_textRelPos = {m_text.getGlobalBounds().size.x/2,175};
  m_text.setPosition(m_background.getPosition()-m_textRelPos);

  sf::Vector2f textInputSize = {background_size.x - 40,50};
  m_lyricsPathInput = new TextInput(textInputSize,m_font);
  m_lyricsPathInput->setPosition(m_background.getPosition() + sf::Vector2f(background_size.x/2, background_size.y/2));
  m_lyricsPathInput->setPlaceholderText("Path to the file");
}

LoadLyricsDialog::~LoadLyricsDialog() {
   if(m_lyricsPathInput)
    delete m_lyricsPathInput; 
}

sf::String LoadLyricsDialog::getPath() {
  if(m_lyricsPathInput)
    return m_lyricsPathInput->getText();
  // else
  return sf::String(); 
}

void LoadLyricsDialog::onTextEntered(char ch){
  if(m_lyricsPathInput->getActive())
    m_lyricsPathInput->onTextEntered(ch);
}


void LoadLyricsDialog::update(const float& dt) {
  DialogBase::update(dt);

  m_lyricsPathInput->update(dt);
}

void LoadLyricsDialog::handleMouseClick(const sf::Vector2f& mousePos) {
  if(m_lyricsPathInput->getGlobalBounds().contains(mousePos))
    m_lyricsPathInput->setActive(true);
}

void LoadLyricsDialog::render(sf::RenderTarget* target) {
  DialogBase::render(target);
  
  m_lyricsPathInput->render(target);
}

void LoadLyricsDialog::setPosition(const sf::Vector2f& pos) {
  DialogBase::setPosition(pos);
  sf::Vector2f background_size = m_background.getSize();
  m_lyricsPathInput->setPosition({m_background.getPosition().x, m_background.getPosition().y + 25});
  // m_songInput->setPosition(m_background.getPosition() + sf::Vector2f(background_size.x/2, background_size.y/2));  
}
