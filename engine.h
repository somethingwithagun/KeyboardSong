#ifndef ENGINE_H_
#define ENGINE_H_

#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <unordered_map>

#include "button.h"
#include "radiobutton.h"
#include "plotwidget.h"
#include "constants.h"
#include "parser.h"
#include "requestsongdialog.h"
#include "loadlyricsdialog.h"

class Engine {
public:
  Engine(int fps = 60);
  ~Engine();

  void start();

private:
  void init();
  void initVariables();
  
  std::string setRandomWord();
  void nextWord();

  float calculateStats(const int mistakes, const int letters) const;

// util
  std::string formatFloat(float value) const;
  std::string getCurrentTime() const;
  bool isDialogOpened(const DialogBase* d) const;
  void updateStatsLabels();
  void setTranslatedWord(const sf::String& translatedWord);


  void moveToNextWord();
  void loadLyrics(const std::string& path);
  sf::String translateWord(std::string word);
  sf::String decodeTranslatedWord(std::string word);
  void onTextEntered(char ch);
  void handleWordSubmit();
  void handleTypedCharacter(char ch);
  void eraseLastTypedLetter();
  void processEvents();
  void handleSongEnd();
  void updateDialogs();
  void updateRequestLyricsDialog();
  void updateLoadLyricsDialog();
  void processMouseInput();
  void handleMouseRelease();
  bool isMouseOver(const Button* button) const;
  void clearRating();
  void openRequestLyricsDialog();
  void openLoadLyricsDialog();
  void toggleTranslatedWord();
  void updateButtons();
  void renderFrame();
  void updateFrameState();

  DialogBase* getActiveDialog();

  bool writeRating(const std::string& time, float rating);

  template<class T> void freeVector(std::vector<T*>& vec);

  int m_wordIndex;
  
  const int m_charSpacing = 20;

	sf::Clock m_dtClock;
	sf::Clock m_timer;

  sf::RenderWindow* m_app = nullptr;
  int m_fps;
  float m_dt;
  bool m_quit = false;
  float m_counterA;
  bool m_mouseDown;
  bool m_isGameOver;
  bool m_isAnyDialogsOpen;
  
  sf::String m_currentWord;
  sf::String m_currentTranslatedWord;
	sf::Text m_wordLabel;
	sf::Text m_mistakesLabel;
	sf::Text m_correctLabel;

  sf::Color m_buttonColor = sf::Color(151, 137, 139);

  Button* m_exitButton = nullptr;
  Button* m_clearRatingButton = nullptr;
  Button* m_requestLyricsButton = nullptr;
  Button* m_loadLyricsButton = nullptr;

  RequestSongDialog* m_requestLyricsDialog = nullptr;
  LoadLyricsDialog* m_loadLyricsDialog = nullptr;

  PlotWidget<sf::String> m_plot;

	int mistakes = 0;
	int summaryChars = 0;
  int m_tryRating;

  std::fstream m_ratingFile;

  sf::Font m_font;

  std::vector<std::string> m_words;
  std::vector<sf::RectangleShape*> m_letterPlaces;
	std::vector<sf::Text*> m_letters;
};

#endif
