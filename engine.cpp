#include "engine.h"

// TODO: Refactor, delete translating and redesign -> portfolio profit!

Engine::Engine(int fps) : m_fps(fps), m_wordLabel(m_font), m_mistakesLabel(m_font), m_correctLabel(m_font) {
  	if(!m_font.openFromFile(NOTOSANS_FONT_PATH)) {
    	std::cerr << "Engine::Engine(int): Could not open font" << std::endl;
		throw std::exception();
	} else {
		m_exitButton = new Button(sf::Vector2f(50,50),"X",m_font,RED);
		m_clearRatingButton = new Button(sf::Vector2f(150,50),"Clear Rating",m_font,m_buttonColor);
		m_requestLyricsButton = new Button(sf::Vector2f(150,50),"Request Song",m_font,m_buttonColor);
		m_loadLyricsButton = new Button(sf::Vector2f(150,50),"Load Lyrics",m_font,m_buttonColor);

		m_wordLabel = sf::Text(m_font, "");
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
	

  m_mistakesLabel.setPosition(sf::Vector2f(m_app->getSize().x/2 + 150, 100));
  m_correctLabel.setPosition(sf::Vector2f(m_app->getSize().x/2 - 150, 100));
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

	updateStatsLabels();

	m_currentTranslatedWord = "";
	
	moveToNextWord();

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
		activeDialog->onTextEntered(ch);
		return;
	}

	if(ch == BACKSPACE_CODE) {
		eraseLastTypedLetter();
	} else if(ch == ENTER_CODE || ch == SPACE_CODE) { // enter or space
		handleWordSubmit();
	} else if(ch > SPACE_CODE && ch < 128) {
		handleTypedCharacter(ch);
	}
}

void Engine::handleWordSubmit() {
	if(m_isGameOver) {
		initVariables();
		m_isGameOver = false;
		return;
	}

	if(m_letters.size() == m_currentWord.getSize() && !m_words.empty()) {
		updateStatsLabels();
		m_tryRating += round(float(summaryChars-mistakes)/(m_timer.restart().asSeconds()*10)); // TODO: improve formula
		moveToNextWord();
	}
}

void Engine::handleTypedCharacter(char ch) {
	if(m_letters.size() >= m_currentWord.getSize()) {
		return;
	}

	m_letters.push_back(new sf::Text(m_font, sf::String(ch)));

	if(ch != m_currentWord[m_letters.size()-1]) {
		m_letters.back()->setFillColor(sf::Color::Red);
		mistakes++;
	}
			
	m_letters.back()->setOrigin(sf::Vector2f(m_letters.back()->getGlobalBounds().size.x/2,m_letters.back()->getGlobalBounds().size.y/2));
	m_letters.back()->setPosition(sf::Vector2f(m_letterPlaces[m_letters.size()-1]->getPosition().x, 300));
}

void Engine::eraseLastTypedLetter() {
	if(!m_letters.empty()){
		delete m_letters.back();
		m_letters.pop_back();
	}
}

void Engine::start() {
  init();
	
  while(m_app->isOpen()) {
		processEvents();
		handleSongEnd();
		updateDialogs();
		processMouseInput();
		updateButtons();
		renderFrame();
		updateFrameState();
	}
	freeVector(m_letterPlaces);
	freeVector(m_letters);
}

void Engine::processEvents() {
	while(const auto e = m_app->pollEvent()) {
		if(e->is<sf::Event::Closed>()) {
			m_app->close();
			continue;
		}

		if(const auto text = e->getIf<sf::Event::TextEntered>()){
			char ch = text->unicode;
			onTextEntered(ch);
		}
	}
}

void Engine::handleSongEnd() {
	if(!m_currentWord.isEmpty() || !m_isGameOver) {
		return;
	}

	std::cout << "*** SONG ENDED ***\n";
	std::cout << "Rating: "<< m_tryRating << '\n';
	std::cout << "Symbols written: "<< summaryChars << '\n';
	std::cout << "Errors: " << mistakes << '\n' << std::flush;

	const std::string curTime = getCurrentTime();
	m_plot.apply(curTime,m_tryRating);
	writeRating(curTime,m_tryRating);

	m_wordLabel.setString("Song ended! Press Enter to restart");
	m_wordLabel.setOrigin(sf::Vector2f(m_wordLabel.getGlobalBounds().size.x/2,m_wordLabel.getGlobalBounds().size.y/2));
	m_wordLabel.setPosition(sf::Vector2f(m_app->getSize().x/2, 200));

	freeVector(m_letterPlaces);
	freeVector(m_letters);
	m_isGameOver = true;
}

void Engine::updateDialogs() {
	DialogBase* activeDialog = getActiveDialog();

	if(!activeDialog) {
		return;
	}


	if(activeDialog == m_requestLyricsDialog) {
		updateRequestLyricsDialog();
	} else if(activeDialog == m_loadLyricsDialog) {
		updateLoadLyricsDialog();
	}
}

void Engine::updateRequestLyricsDialog() {
	m_requestLyricsDialog->update(m_dt);
	const DialogBase::Result res = m_requestLyricsDialog->getResult();
	if(res == DialogBase::Accepted) {
		const std::string author = m_requestLyricsDialog->getAuthorName().toAnsiString();
		const std::string songname = m_requestLyricsDialog->getSongName().toAnsiString();

		if(author.empty() || songname.empty()) {
			std::cerr << "author or songname cannot be empty" << std::endl;
		} else {
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
}

void Engine::updateLoadLyricsDialog() {
	if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		m_loadLyricsDialog->handleMouseClick(static_cast<sf::Vector2f>(sf::Mouse::getPosition(*m_app)));
	}
	
	m_loadLyricsDialog->update(m_dt);
	
	const DialogBase::Result res = m_loadLyricsDialog->getResult();
	
	if(res == DialogBase::Accepted) {
		const std::string filepath = m_loadLyricsDialog->getPath().toAnsiString();
		if(filepath.empty()) {
			std::cerr << "cannot read the empty path" << std::endl;
		} else {
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

void Engine::processMouseInput() {
	if(m_isAnyDialogsOpen) {
		return;
	}

	if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		m_mouseDown = true;
	}

	if(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && m_mouseDown) {
		handleMouseRelease();
	}
}

void Engine::handleMouseRelease() {
	m_mouseDown = false;

	if(isMouseOver(m_exitButton) && m_exitButton->isReady()) {
		m_exitButton->setPressed(true);
		m_quit = true;
		m_app->close();
		return;
	}

	if(isMouseOver(m_clearRatingButton) && m_clearRatingButton->isReady()) {
		m_clearRatingButton->setPressed(true);
		clearRating();
		return;
	}

	if(isMouseOver(m_requestLyricsButton) && m_requestLyricsButton->isReady()) {
		m_requestLyricsButton->setPressed(true);
		openRequestLyricsDialog();
		return;
	}

	if(isMouseOver(m_loadLyricsButton) && m_loadLyricsButton->isReady()) {
		m_loadLyricsButton->setPressed(true);
		openLoadLyricsDialog();
		return;
	}
}

bool Engine::isMouseOver(const Button* button) const {
	return button && button->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition()));
}

void Engine::clearRating() {
	std::ofstream ratingFile(RATING_TXT_PATH);
	if(ratingFile.is_open()){
		std::cout << "Cleared rating at " << getCurrentTime() << std::endl;
		ratingFile.close();
		m_plot.reset();
	}
}

void Engine::openRequestLyricsDialog() {
	m_requestLyricsDialog = new RequestSongDialog("Request Song", sf::Vector2f(600,400),sf::Vector2f(150,50),sf::Color(151, 137, 139),m_font);
	m_requestLyricsDialog->setPosition(sf::Vector2f(m_app->getSize().x/2,m_app->getSize().y/2));
	m_requestLyricsDialog->show();
	m_isAnyDialogsOpen = true;
}

void Engine::openLoadLyricsDialog() {
	m_loadLyricsDialog = new LoadLyricsDialog("Load Lyrics", sf::Vector2f(600,400),sf::Vector2f(150,50),sf::Color(151, 137, 139),m_font);
	m_loadLyricsDialog->setPosition(sf::Vector2f(m_app->getSize().x/2,m_app->getSize().y/2));
	m_loadLyricsDialog->show();
	m_isAnyDialogsOpen = true;
}

void Engine::updateButtons() {
	m_exitButton->update(m_dt);
	m_clearRatingButton->update(m_dt);
	m_requestLyricsButton->update(m_dt);
	m_loadLyricsButton->update(m_dt);
}

void Engine::renderFrame() {
	m_app->clear(sf::Color(26, 26, 26));
	m_app->draw(m_wordLabel);
	m_app->draw(m_mistakesLabel);
	m_app->draw(m_correctLabel);
	m_app->draw(m_plot);
	m_app->draw(*m_exitButton);
	m_app->draw(*m_clearRatingButton);
	m_app->draw(*m_requestLyricsButton);
	m_app->draw(*m_loadLyricsButton);

	for(int i = 0; i < m_letterPlaces.size(); i++) {
		m_app->draw(*m_letterPlaces[i]);
	}

	for(int i = 0; i < m_letters.size(); i++) {
		m_app->draw(*m_letters[i]);
	}

	if(m_requestLyricsDialog) {
		m_requestLyricsDialog->render(m_app);
	}

	if(m_loadLyricsDialog) {
		m_loadLyricsDialog->render(m_app);
	}

	m_app->display();
}

void Engine::updateFrameState() {
	m_dt = m_dtClock.restart().asSeconds();
	m_counterA += m_dt;
}

void Engine::loadLyrics(const std::string& path) {
	
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

std::string Engine::formatFloat(float value) const {
	if(value < 0.1)
		return "0.00";
	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << value;
	return stream.str();
}

std::string Engine::setRandomWord() {
	srand(time(0));
	return m_words[rand() % (m_words.size())];
}

sf::String Engine::translateWord(std::string word) {
	return sf::String(word);
}

sf::String Engine::decodeTranslatedWord(std::string word) {
	return sf::String(word);
}


void Engine::nextWord() {
	if(m_words.size() == m_wordIndex) {
		m_currentWord = "";
	} else {
		m_currentWord = m_words[m_wordIndex];
		m_wordIndex++;
	}
}

std::string Engine::getCurrentTime() const {
	time_t t = time(0);
	tm* t1 = localtime(&t);
	int h = t1->tm_hour,
			m = t1->tm_min,
			s = t1->tm_sec;
	return ((h<10?"0":"") + std::to_string(h) + ':' +(m<10?"0":"") + std::to_string(m) + ':' + (s<10?"0":"")+ std::to_string(s));
}

void Engine::updateStatsLabels() {
	const float stat = calculateStats(mistakes, summaryChars);

	m_mistakesLabel.setString(formatFloat(stat) + "%");
	m_mistakesLabel.setOrigin(sf::Vector2f(m_mistakesLabel.getGlobalBounds().size.x / 2, m_mistakesLabel.getGlobalBounds().size.y / 2));

	m_correctLabel.setString(formatFloat(100.f - stat) + "%");
	m_correctLabel.setOrigin(sf::Vector2f(m_correctLabel.getGlobalBounds().size.x / 2, m_correctLabel.getGlobalBounds().size.y / 2));
}


void Engine::moveToNextWord() {
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

bool Engine::writeRating(const std::string& time, float rating) {
	m_ratingFile.open(RATING_TXT_PATH, std::ios_base::out | std::ios_base::app);
	if(!m_ratingFile.is_open()) {
		std::cerr << "Engine::writeRating(std::string, float): error in opening rating file" << std::endl;
		return false;
	}
	m_ratingFile << rating << ' ' << time << '\n';
	m_ratingFile.close();
	return true;
}
