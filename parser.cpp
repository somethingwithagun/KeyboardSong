#include "parser.h"

AzLyricsParser::AzLyricsParser() {
	m_curl = curl_easy_init();
	if(m_curl) {
	    curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, 0);
	    curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, 0);
	    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, writeCallback);
	    // words = parse_lyrics(response);
	}
	else {std::cerr << "couldn't start curl!" << std::endl;}
}

AzLyricsParser::~AzLyricsParser() {
  if(m_curl)
    curl_easy_cleanup(m_curl);
}

std::vector<std::string> AzLyricsParser::getLyricsList(std::string author, std::string name) {
	format(author);
	format(name);

	std::string fullAddress = m_address+author+'/'+name +".html";
  if(m_curl) {
    curl_easy_setopt(m_curl, CURLOPT_URL, fullAddress.c_str());
    std::string response;
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(m_curl);
    if(res != CURLE_OK) {
      std::cerr << "Failed to get lyrics: " << curl_easy_strerror(res) << std::endl;
    } else {
      std::cout << "Lyrics loaded" << std::endl;
      return parseLyrics(response);
    }

  } else std::cerr << "curl isn't started" << std::endl;

  return std::vector<std::string>();
}

void AzLyricsParser::divideWords(const std::string& str, std::vector<std::string>& lyrics) {
	int word_start = 0, word_end = 0;
	while(str[word_start] == ' ' || str[word_start] == '\n'|| str[word_start] == '\r') word_start++;

	for(int word_end = word_start; word_end < str.size(); word_end++) {
			if(str[word_end] == ' ' || str[word_end] == '\n'|| str[word_end] == '\r') {
					while(str[word_start] == ' ' || str[word_start] == '\n'|| str[word_start] == '\r') word_start++;

					lyrics.push_back(str.substr(word_start,(word_end - word_start)));
					word_end++; // skip the space
					word_start = word_end;
			}
	}
}

void AzLyricsParser::format(std::string& str) {
	std::string tmp;
	for(int i = 0; i < str.size(); i++)
		if(isalpha(str[i]) || isdigit(str[i]))
			tmp+=tolower(str[i]);

	str = tmp;
}

std::vector<std::string> AzLyricsParser::parseLyrics(const std::string& raw_response) {
	int start_index = 0, end_index;
	std::vector<std::string> lyrics;
	if((start_index = raw_response.find("<div class=\"ringtone\">")) != std::string::npos) {
			int index = start_index = raw_response.find("<b>", start_index);

			index += 3;
			std::string title;
			
			while(raw_response[index] != '<')
					title += raw_response[index++];

			title = title.substr(1,title.size()-2);
			title += '\n';

			divideWords(title,lyrics);
			
			index = start_index = raw_response.find("<div>", start_index);
			end_index = raw_response.find("</div>", start_index);

			std::string words;
			for(index; index < end_index; index++) {
					if(raw_response[index] == '<') {
							while(raw_response[index] != '>')
									index++;
							index++;
							words+= ' ';
					} if(raw_response[index] != '\n') words += raw_response[index];
			}
			words+= '\n';

			divideWords(words,lyrics);

	}
	return lyrics;
}

size_t AzLyricsParser::writeCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}
