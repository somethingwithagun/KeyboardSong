#ifndef LOADLYRICSDIALOG_H_
#define LOADLYRICSDIALOG_H_

#include "dialogbase.h"
#include "textinput.h"

class LoadLyricsDialog : public DialogBase {
public:
  LoadLyricsDialog(const sf::String& text, const sf::Vector2f background_size, const sf::Vector2f buttons_size, const sf::Color buttons_color,const sf::Font& font);
  ~LoadLyricsDialog();

  sf::String getPath();

  virtual void onTextEntered(char ch) override;

  virtual void update(const float& dt) override;
	virtual void render(sf::RenderTarget* target) override;
  virtual void handleMouseClick(const sf::Vector2f& mousePos) override;
  
  virtual void setPosition(const sf::Vector2f& pos);
  private:
  TextInput* m_lyricsPathInput = nullptr;
};

#endif