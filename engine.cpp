#include "engine.h"

// TODO: Refactor, delete translating and redesign -> portfolio profit!

Engine::Engine(int fps) : m_fps(fps), m_wordLabel(m_font), m_translatedWordLabel(m_font), m_mistakesLabel(m_font), m_correctLabel(m_font) {
  if(!m_font.openFromFile(NOTOSANS_FONT_PATH))
    std::cerr << "Engine::Engine(int): Could not open font" << std::endl;
	else {
		m_exitButton = new Button(sf::Vector2f(50,50),"X",m_font,RED);
		m_clearRatingButton = new Button(sf::Vector2f(150,50),"Clear Rating",m_font,m_buttonColor);
		m_requestLyricsButton = new Button(sf::Vector2f(150,50),"Request Song",m_font,m_buttonColor);
		m_loadLyricsButton = new Button(sf::Vector2f(150,50),"Load Lyrics",m_font,m_buttonColor);

		m_wordLabel = sf::Text(m_font, "");
		m_translatedWordLabel = sf::Text(m_font, "");  
		m_mistakesLabel = sf::Text(m_font,"0.00%");
		m_mistakesLabel.setFillColor(RED);

		m_correctLabel = sf::Text(m_font, "0.00%");
		m_correctLabel.setFillColor(GREEN);
	}
	m_mistakesLabel.setOrigin(sf::Vector2f(m_mistakesLabel.getGlobalBounds().size.x/2,m_mistakesLabel.getGlobalBounds().size.y/2));

	m_plot.setSize(sf::Vector2f(1000,400));
	m_plot.setFont(&m_font);
  m_plot.setOrigin(sf::Vector2f(m_plot.getSize().x/2,m_plot.getSize().y/2));
}

Engine::~Engine() {	
  delete m_app;
	if(m_exitButton)
		delete m_exitButton;

	if(m_clearRatingButton)
		delete m_clearRatingButton;
	
	if(m_requestLyricsButton)
		delete m_requestLyricsButton;
	
	if(m_loadLyricsButton)
		delete m_loadLyricsButton;
	
	if(m_translateWordButton)
		delete m_translateWordButton;
}

void Engine::init() {
  m_app = new sf::RenderWindow(sf::VideoMode(sf::Vector2u(1920,1080),32), "Keyboard Trainer controller", sf::Style::Default, sf::State::Fullscreen);
  m_app->setFramerateLimit(m_fps);
	
	if(m_exitButton)
		m_exitButton->setPosition(sf::Vector2f(m_app->getSize().x-25, 25));
	
	if(m_clearRatingButton)
		m_clearRatingButton->setPosition(sf::Vector2f(m_app->getSize().x/2 - 160-80, m_app->getSize().y-m_clearRatingButton->getSize().y/2));
	
	if(m_requestLyricsButton)
		m_requestLyricsButton->setPosition(sf::Vector2f(m_app->getSize().x/2 - 80, m_app->getSize().y-m_requestLyricsButton->getSize().y/2));
	
	if(m_loadLyricsButton)
		m_loadLyricsButton->setPosition(sf::Vector2f(m_app->getSize().x/2 + 80, m_app->getSize().y-m_loadLyricsButton->getSize().y/2));
	
	if(m_translateWordButton)
		m_translateWordButton->setPosition(sf::Vector2f(m_app->getSize().x/2 +160 + 80, m_app->getSize().y-m_translateWordButton->getSize().y/2));

  m_mistakesLabel.setPosition(sf::Vector2f(m_app->getSize().x/2 + 150, 100));
  m_correctLabel.setPosition(sf::Vector2f(m_app->getSize().x/2 - 150, 100));
	
	m_translatedWordLabel.setOrigin(sf::Vector2f(m_translatedWordLabel.getGlobalBounds().size.x/2,m_translatedWordLabel.getGlobalBounds().size.y/2));
	m_translatedWordLabel.setPosition(sf::Vector2f(m_app->getSize().x/2, 425));

  m_plot.setPosition(sf::Vector2f(m_app->getSize().x/2, m_app->getSize().y/2 + 200));
  m_plot.loadFromFile(RATING_TXT_PATH);

	initVariables();

}

void Engine::initVariables() {
  m_dt = 0.f;
	m_mouseDown = false;
  m_wordIndex = 0;
	m_counterA = 0.f;
	m_tryRating = 0;
	summaryChars = 0;
	mistakes = 0;
	m_tryRating = 0;
	m_isAnyDialogsOpen = false;
	m_isGameOver = false;
	m_translatePressed = false;
	
	if(m_translateWordButton) {
		m_translateWordButton->setText("Translate");
		m_translateWordButton->setFillColor(m_buttonColor);
	}

	float stat = calculateStats(mistakes,summaryChars);

	m_mistakesLabel.setString(ftostr(stat)+"%");
	m_mistakesLabel.setOrigin(sf::Vector2f(m_mistakesLabel.getGlobalBounds().size.x/2,m_mistakesLabel.getGlobalBounds().size.y/2));

	m_correctLabel.setString(ftostr(100.f - stat)+"%");
	m_correctLabel.setOrigin(sf::Vector2f(m_correctLabel.getGlobalBounds().size.x/2,m_correctLabel.getGlobalBounds().size.y/2));

	m_currentTranslatedWord = "";
	
	changeWord();

	m_dtClock.restart();
	m_timer.restart();
}

bool Engine::isDialogOpened(const DialogBase* d) const {
	return d && d->getIsOpen();
} 

DialogBase* Engine::getActiveDialog() {
	if (isDialogOpened(m_requestLyricsDialog)) {
		return m_requestLyricsDialog;
	} else if (isDialogOpened(m_loadLyricsDialog)) {
		return m_loadLyricsDialog;
	} else return nullptr;
}

void Engine::onTextEntered(char ch) {
	DialogBase* activeDialog = getActiveDialog();

	if(activeDialog) {
		activeDialog->symEntered(ch);
	}

	// else
	if(ch == BACKSPACE_CODE) {
		symErase();
	} else if(ch == ENTER_CODE || ch == SPACE_CODE) { // enter or space
		if(m_isGameOver) {
			initVariables();
			m_isGameOver = false;
		}else if(m_letters.size() == m_currentWord.getSize() && !m_words.empty()) {
			float stat = calculateStats(mistakes,summaryChars);

			m_mistakesLabel.setString(ftostr(stat)+"%");
			m_mistakesLabel.setOrigin(sf::Vector2f(m_mistakesLabel.getGlobalBounds().size.x/2,m_mistakesLabel.getGlobalBounds().size.y/2));

			m_correctLabel.setString(ftostr(100.f - stat)+"%");
			m_correctLabel.setOrigin(sf::Vector2f(m_correctLabel.getGlobalBounds().size.x/2,m_correctLabel.getGlobalBounds().size.y/2));
			m_tryRating += round(float(summaryChars-mistakes)/(m_timer.restart().asSeconds()*10)); // TODO: improve formula

			changeWord();
			}else {}
			
		}

	}
	else if(ch->unicode > SPACE_CODE && ch->unicode < 128) {
		if(m_letters.size() < m_currentWord.getSize()) {
			m_letters.push_back(new sf::Text(m_font, sf::String(ch->unicode)));

			if(ch->unicode != m_currentWord[m_letters.size()-1]) {
				m_letters.back()->setFillColor(sf::Color::Red);
				mistakes++;
			}
			
			m_letters.back()->setOrigin(sf::Vector2f(m_letters.back()->getGlobalBounds().size.x/2,m_letters.back()->getGlobalBounds().size.y/2));
			m_letters.back()->setPosition(sf::Vector2f(m_letterPlaces[m_letters.size()-1]->getPosition().x, 300));
		}
	}
}

void Engine::symErase() {
	if(!m_letters.empty()){
		delete m_letters.back();
		m_letters.pop_back();
	}
}

void Engine::start() {
  init();
	
  while(m_app->isOpen()) {
		while(const auto e = m_app->pollEvent()) {
			if(e->is<sf::Event::Closed>())
				m_app->close();
			else {
				if(const auto text = e->getIf<sf::Event::TextEntered>()){
					char ch = text->unicode;
					onTextEntered(ch);
				}
			}
		}
		if(m_currentWord.isEmpty()) {
			std::cout << "*** SONG ENDED ***\n";
			std::cout << "Rating: "<< m_tryRating << '\n';
			std::cout << "Symbols written: "<< summaryChars << '\n';
			std::cout << "Errors: " << mistakes << '\n' << std::flush;

			std::string curTime = currentTime();
			m_plot.apply(curTime,m_tryRating);
			writeRating(curTime,m_tryRating);

			m_wordLabel.setString("Song ended! Press Enter to restart");
			m_wordLabel.setOrigin(sf::Vector2f(m_wordLabel.getGlobalBounds().size.x/2,m_wordLabel.getGlobalBounds().size.y/2));
			m_wordLabel.setPosition(sf::Vector2f(m_app->getSize().x/2, 200));

			freeVector(m_letterPlaces);
			freeVector(m_letters);

			m_isGameOver = true;
		}



		if(m_isAnyDialogsOpen) {
			if(m_requestLyricsDialog && m_requestLyricsDialog->getIsOpen()) {
				m_requestLyricsDialog->update(m_dt);
				DialogBase::Result res = m_requestLyricsDialog->getResult();
				if(res == DialogBase::Accepted) {
					std::string author = m_requestLyricsDialog->getAuthorName().toAnsiString(); /* get author name from text field */
					std::string songname = m_requestLyricsDialog->getSongName().toAnsiString(); /* get song name from text field */

					if(author.empty() || songname.empty())
						std::cerr << "author or songname cannot be empty" << std::endl;
					else {
						AzLyricsParser parser;
						m_words = parser.getLyricsList(author, songname);
						initVariables();
					}
				}
				if(res != DialogBase::None) {
					m_requestLyricsDialog->close();
					delete m_requestLyricsDialog;
					m_requestLyricsDialog = nullptr;
					m_isAnyDialogsOpen = false;
				}
			} else if(m_loadLyricsDialog && m_loadLyricsDialog->getIsOpen()) {
				m_loadLyricsDialog->update(m_dt);
				DialogBase::Result res = m_loadLyricsDialog->getResult();
				if(res == DialogBase::Accepted) {
					std::string filepath = m_loadLyricsDialog->getPath().toAnsiString(); /* get song name from text field */
					if(filepath.empty())
						std::cerr << "cannot read the empty path" << std::endl;
					else{
						loadLyrics(filepath);
						initVariables();
					}
				}
				if(res != DialogBase::None) {
					m_loadLyricsDialog->close();
					delete m_loadLyricsDialog;
					m_loadLyricsDialog = nullptr;
					m_isAnyDialogsOpen = false;
				}
			}
			
		} if(!m_isAnyDialogsOpen){		
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) m_mouseDown = true;

			// act on mouse release
			if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && m_mouseDown) {
				m_mouseDown = false;
				if (m_exitButton->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition()))) {
					if (m_exitButton->isReady()) {
						m_exitButton->setPressed(true);
						
						m_quit = true;
						m_app->close();
					}
				} else if(m_clearRatingButton->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition()))) {
					if(m_clearRatingButton->isReady()) {
						m_clearRatingButton->setPressed(true);
						
						std::ofstream ratingFile(RATING_TXT_PATH);
						if(ratingFile.is_open()){
							std::cout << "Cleared rating at " << currentTime() << std::endl;
							ratingFile.close();

							m_plot.reset();
						}

					}
				} else if(m_requestLyricsButton->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition()))) {
					if(m_requestLyricsButton->isReady()) {
						m_requestLyricsButton->setPressed(true);
						
						// open dialog with author and song name text fields
						m_requestLyricsDialog = new RequestSongDialog("Request Song", sf::Vector2f(600,400),sf::Vector2f(150,50),sf::Color(151, 137, 139),m_font);
						m_requestLyricsDialog->setPosition(sf::Vector2f(m_app->getSize().x/2,m_app->getSize().y/2));
						m_requestLyricsDialog->show();
						m_isAnyDialogsOpen = true;

						// assert(0 && "not implemented");
					}
				} else if(m_loadLyricsButton->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition()))) {
						if(m_loadLyricsButton->isReady()) {
							m_loadLyricsButton->setPressed(true);
							
							// open dialog with author and song name text fields
							m_loadLyricsDialog = new LoadLyricsDialog("Load Lyrics", sf::Vector2f(600,400),sf::Vector2f(150,50),sf::Color(151, 137, 139),m_font);
							m_loadLyricsDialog->setPosition(sf::Vector2f(m_app->getSize().x/2,m_app->getSize().y/2));
							m_loadLyricsDialog->show();
							m_isAnyDialogsOpen = true;

							// assert(0 && "not implemented");
						}
				} else if(m_translateWordButton->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition()))) {
					if(m_translateWordButton->isReady() && !m_isGameOver && !m_words.empty()) {
						if(!m_translatePressed) {
							m_translateWordButton->setPressed(true);
							
							// change button state
							m_translateWordButton->setFillColor(RED);
							m_translateWordButton->setText("Hide");

							m_currentTranslatedWord = translateWord(m_currentWord);
							m_translatedWordLabel.setString(m_currentTranslatedWord);
							m_translatedWordLabel.setOrigin(sf::Vector2f(m_translatedWordLabel.getGlobalBounds().size.x/2,m_translatedWordLabel.getGlobalBounds().size.y/2));
							m_translatedWordLabel.setPosition(sf::Vector2f(m_app->getSize().x/2.0f, 425));

							m_translatePressed = true;
						} else {
							m_translateWordButton->setPressed(true);
							
							// change button state
							m_translateWordButton->setFillColor(m_buttonColor);
							m_translateWordButton->setText("Translate");

							m_currentTranslatedWord = "";
							m_translatedWordLabel.setString(m_currentTranslatedWord);
							m_translatePressed = false;
						}
					}
				}
			}
		}
		m_exitButton->update(m_dt);
		m_clearRatingButton->update(m_dt);
		m_requestLyricsButton->update(m_dt);
		m_loadLyricsButton->update(m_dt);
		m_translateWordButton->update(m_dt);
		// if(m_enableTranslate->getChecked()) m_enableTranslate->update(m_dt);

		m_app->clear(sf::Color(26, 26, 26));
		m_app->draw(m_wordLabel);

		// if(m_enableTranslate->getChecked()) m_app->draw(m_translatedWordLabel);
		m_app->draw(m_translatedWordLabel);

		m_app->draw(m_mistakesLabel);
		m_app->draw(m_correctLabel);
		m_app->draw(m_plot);
		m_app->draw(*m_exitButton);
		m_app->draw(*m_clearRatingButton);
		m_app->draw(*m_requestLyricsButton);
		m_app->draw(*m_loadLyricsButton);
		m_app->draw(*m_translateWordButton);

		for(int i = 0; i < m_letterPlaces.size(); i++)
				m_app->draw(*m_letterPlaces[i]);
		for(int i = 0; i < m_letters.size(); i++)
				m_app->draw(*m_letters[i]);

		if(m_requestLyricsDialog)
			m_requestLyricsDialog->render(m_app);

		if(m_loadLyricsDialog)
			m_loadLyricsDialog->render(m_app);
	
		m_app->display();

		m_dt = m_dtClock.restart().asSeconds();
		m_counterA += m_dt;
	}
	freeVector(m_letterPlaces);
	freeVector(m_letters);
}

void Engine::loadLyrics(std::string path) {
	
	m_words.clear();

	std::ifstream in(path);

	if(!in.is_open()){
		std::cout << "Unable to open file" << std::endl;
		return;
	}
	std::string word;
	while(in >> word)
		m_words.push_back(word);

	in.close();

}

float Engine::calculateStats(const int mistakes, const int letters) const {
	if(letters == 0)
		return 0;
	float num = (float(mistakes)/float(letters)) * 100;
	return num;	
}

std::string Engine::ftostr(float x) const {
	if(x < 0.1)
		return "0.00";
	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << x;
	return stream.str();
}

std::string Engine::setRandomWord() {
	srand(time(0));
	return m_words[rand() % (m_words.size())];
}


void Engine::nextWord() {
	if(m_words.size() == m_wordIndex) {
		m_currentWord = "";
	} else {
		m_currentWord = m_words[m_wordIndex];
		m_wordIndex++;
	}
}

std::string Engine::currentTime() {
	time_t t = time(0);
	tm* t1 = localtime(&t);
	int h = t1->tm_hour,
			m = t1->tm_min,
			s = t1->tm_sec;
	return ((h<10?"0":"") + std::to_string(h) + ':' +(m<10?"0":"") + std::to_string(m) + ':' + (s<10?"0":"")+ std::to_string(s));
}

void Engine::changeWord() {
	nextWord();

	m_wordLabel.setString(m_currentWord);
	m_wordLabel.setOrigin(sf::Vector2f(m_wordLabel.getGlobalBounds().size.x/2,m_wordLabel.getGlobalBounds().size.y/2));
	m_wordLabel.setPosition(sf::Vector2f(m_app->getSize().x/2.0, 200));
	
	freeVector(m_letterPlaces);

	int word_container_width = m_currentWord.getSize()*(100 +m_charSpacing) - m_charSpacing;
	int offset = m_app->getSize().x/2 - word_container_width/2 + 50;
	for(int i = 0; i < m_currentWord.getSize(); i++) {
			m_letterPlaces.push_back(new sf::RectangleShape(sf::Vector2f(100,15)));
			m_letterPlaces[i]->setOrigin(sf::Vector2f(m_letterPlaces[i]->getSize().x/2,m_letterPlaces[i]->getSize().y/2));
			m_letterPlaces[i]->setPosition(sf::Vector2f((m_charSpacing+100) * i + offset, 350));
	}
	freeVector(m_letters);

	summaryChars += m_currentWord.getSize();
}

template<class T> void Engine::freeVector(std::vector<T*>& vec) {			
	while(!vec.empty()) {
    delete vec.back();
    vec.pop_back();
  }
}

bool Engine::writeRating(std::string time, float rating) {
	m_ratingFile.open(RATING_TXT_PATH, std::ios_base::out | std::ios_base::app);
	if(!m_ratingFile.is_open()) {
		std::cerr << "Engine::writeRating(std::string, float): error in opening rating file" << std::endl;
		return false;
	}
	m_ratingFile << rating << ' ' << time << '\n';
	m_ratingFile.close();
	return true;
}
