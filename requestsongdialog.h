#ifndef SONGREQUESTDIALOG_H_
#define SONGREQUESTDIALOG_H_

#include "dialogbase.h"
#include "textinput.h"

class RequestSongDialog : public DialogBase {
public:
  RequestSongDialog(const sf::String& text, const sf::Vector2f background_size, const sf::Vector2f buttons_size, const sf::Color buttons_color,const sf::Font& font);
  ~RequestSongDialog();

  sf::String getAuthorName();
  sf::String getSongName();

  void symEntered(char c);
  void symErase();
  
  TextInput* getActiveInput();

  bool isTextSymbol(char ch) const;

  virtual void update(const float& dt) override;
	virtual void render(sf::RenderTarget* target) override;

  virtual void setPosition(const sf::Vector2f& pos);
private:
  TextInput m_authorInput;
  TextInput m_songInput;
};

#endif