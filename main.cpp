#include <iostream>
#include "engine.h"

/* TODO:
	* Add word-by-word translation
	* Add rating mark if user left
*/

bool first_enter = true;


// std::string calculate_errors(const int mistakes, const int letters) {
// 	float num = (float(mistakes)/float(letters)) * 100;
// 	char buf[10];
// 	sprintf(buf,"%.2f",num);
// 	return std::string(buf);
// }

// std::string set_random_word() {
// 	srand(time(0));
// 	return words[rand() % (words.size())];
// }

// std::string next_word() {
// 	static int index = 0;
// 	if(index == words.size()) return "";
// 	return words[index++];
// }

// void change_word(sf::String& word,sf::RenderWindow& app, sf::Text& word_label,std::vector<sf::RectangleShape*>& letter_places,std::vector<sf::Text*>& letters, int& summary_chars){
// 	word_label.setString(word);
// 	word_label.setOrigin(word_label.getGlobalBounds().width/2,word_label.getGlobalBounds().height/2);
// 	word_label.setPosition(app.getSize().x/2, 200);
	
// 	for(int i = letter_places.size()-1; i >=0; i--) {
// 			delete letter_places[i];
// 			letter_places.pop_back();
// 	}        

// 	int word_container_width = word.getSize()*(100 +char_spacing) - char_spacing;
// 	int offset = app.getSize().x/2 - word_container_width/2 + 50;
// 	for(int i = 0; i < word.getSize(); i++) {
// 			letter_places.push_back(new sf::RectangleShape(sf::Vector2f(100,15)));
// 			letter_places[i]->setOrigin(letter_places[i]->getSize().x/2,letter_places[i]->getSize().y/2);
// 			letter_places[i]->setPosition(sf::Vector2f((char_spacing+100) * i + offset, 350));
// 	}
			
// 	for(int i = letters.size()-1; i >=0; i--) {
// 			delete letters[i];
// 			letters.pop_back();
// 	}       

// 	summary_chars += word.getSize();
// }

bool check_last_day(){
	time_t t = time(0);
	tm* t1 = localtime(&t);
	int cd = t1->tm_mday; // current day

	std::fstream in(LAST_DAY_TXT_PATH, std::ios_base::in);
	if(!in.is_open()) {
		in.open(LAST_DAY_TXT_PATH, std::ios_base::out);
		in << cd;
		return false;
	}

	int ld; // last day
	in >> ld;
	if(ld != cd) {
		first_enter = false;
		in.close();
		{ // clears files
			in.open(LAST_DAY_TXT_PATH, std::ios_base::out | std::ios_base::ate);
			in << cd;
			in.close();

			in.open(RATING_TXT_PATH, std::ios_base::out | std::ios_base::ate);
			in.close();
		}

		return false;
	}
	return true;
}

int main() {
	check_last_day();
	Engine engine(50);
	engine.start();

	// sf::RenderWindow app(sf::VideoMode(0,0),"Keyboard Trainer controller", sf::Style::Default | sf::Style::Fullscreen);
	// app.setFramerateLimit(30);

	// std::vector<sf::RectangleShape*> letter_places;
	// std::vector<sf::Text*> letters;
	
	// sf::Font font;
	// if(!font.loadFromFile(NOTOSANS_FONT_PATH)) {
	// 		std::cerr << "Could not open font" << std::endl;
	// 		return EXIT_FAILURE;
	// }

	// sf::String word;
	// sf::Text word_label("",font),
	// 				mistakes_label("0.0%",font);
	
	// mistakes_label.setOrigin(mistakes_label.getGlobalBounds().width/2,mistakes_label.getGlobalBounds().height/2);
	// mistakes_label.setPosition(app.getSize().x - 200, 200);
	
	// int mistakes = 0;
	// int summary_chars = 0;

	// PlotWidget<sf::String> plot;
	// plot.setOrigin(plot.getSize().x/2,plot.getSize().y/2);
	// plot.setPosition(app.getSize().x/2, app.getSize().y/2 + 200);
	// plot.loadFont(NOTOSANS_FONT_PATH);

	// std::vector<std::pair<sf::String, float>> rating_data;

	// int summary_rating = 0;
	
	// check_last_day();
	// plot.loadFromFile(RATING_TXT_PATH);

	// word = next_word();
	// change_word(word, app, word_label, letter_places, letters, summary_chars);

	// sf::Clock clock;
	// clock.restart();

	// while(app.isOpen()) {
	// 		sf::Event e;    
	// 		while(app.pollEvent(e)) {
	// 				if(e.type == sf::Event::Closed)
	// 						app.close();
	// 				else if(e.type == sf::Event::TextEntered) {
	// 						if(e.text.unicode == '\b') {
	// 								if(!letters.empty()) {
	// 										delete letters.back();
	// 										letters.pop_back();
	// 								}
	// 						} else if(e.text.unicode == '\r' || e.text.unicode == ' ') {
	// 								if(letters.size() == word.getSize()) {
	// 										mistakes_label.setString(calculate_errors(mistakes,summary_chars) + "%");
	// 										word = next_word();
	// 										if(word.isEmpty()){
	// 												std::clog << "Song ended" << std::endl;
	// 												float rating = round(float(summary_chars-mistakes)*100/clock.getElapsedTime().asSeconds());
	// 												summary_rating += rating;
	// 												time_t t = time(0);
	// 												tm* t1 = localtime(&t);
	// 												int h = t1->tm_hour,
	// 														m = t1->tm_min,
	// 														s = t1->tm_sec;
	// 												std::string time = (h<10?"0":"") + std::to_string(h) + ':' +(m<10?"0":"") + std::to_string(m) + ':' + (s<10?"0":"")+ std::to_string(s);
	// 												write_rating(rating, time,t1->tm_mday);
	// 												plot.apply(time,rating);
	// 										}
	// 										change_word(word, app, word_label, letter_places, letters, summary_chars);
	// 								}

	// 						} else if(e.text.unicode < 128) {
	// 								if(letters.size() < word.getSize()) {
	// 										letters.push_back(new sf::Text(sf::String(e.text.unicode), font));

	// 										if(e.text.unicode != word[letters.size()-1]) {
	// 												letters.back()->setFillColor(sf::Color::Red);
	// 												mistakes++;
	// 										}
											
	// 										letters.back()->setOrigin(letters.back()->getGlobalBounds().width/2,letters.back()->getGlobalBounds().height/2);
	// 										letters.back()->setPosition(letter_places[letters.size()-1]->getPosition().x, 300);
	// 								}
	// 						}
	// 				}
	// 		}

	// 		app.clear();
	// 		app.draw(word_label);
	// 		app.draw(mistakes_label);
	// 		app.draw(plot);

	// 		for(int i = 0; i < letter_places.size(); i++)
	// 				app.draw(*letter_places[i]);
	// 		for(int i = 0; i < letters.size(); i++)
	// 				app.draw(*letters[i]);

	// 		app.display();
	// }

	// for(int i = letter_places.size()-1; i >=0; i--) {
	// 		delete letter_places[i];
	// 		letter_places.pop_back();
	// }        

	// for(int i = letters.size()-1; i >=0; i--) {
	// 		delete letters[i];
	// 		letters.pop_back();
	// }        
	// std::cout << summary_rating << std::endl;

	return 0; // delete later




	// curl = curl_easy_init();
	// if(curl) {
	//     curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:5500/script.php");
	//     curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
	//     curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);

	//     struct curl_slist *headers = NULL;
	//     headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
	//     curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	//     std::string username;
	//     std::cin >> username;

	//     std::string postData = "score=1000&username=" + username;

	//     curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());

	//     res = curl_easy_perform(curl);
	//     if(res != CURLE_OK) {
	//         std::cerr << "Failed to post score: " << curl_easy_strerror(res) << std::endl;
	//     }

	//     curl_easy_cleanup(curl);
	// }

	return 0;
}

// (correct/time)*100%