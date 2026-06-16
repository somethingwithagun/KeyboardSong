#ifndef PARSER_H_
#define PARSER_H_
#include <curl/curl.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
class AzLyricsParser {
public:
  AzLyricsParser();
  ~AzLyricsParser();

  /// @brief Requests azlyrics.com site to get lyrics of specified song
  /// @param url author and songname without spaces. No need to set the full address
  /// cause the function adds "https://www.azlyrics.com/lyrics/" prefix to url
  /// @return list of std::string which are the songs lyrics
  std::vector<std::string> getLyricsList(std::string author, std::string name);

private:
  void divideWords(const std::string& str, std::vector<std::string>& lyrics);  
  void format(std::string& str);
  std::vector<std::string> parseLyrics(const std::string& raw_response);
  static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp);

  std::string m_address = "https://www.azlyrics.com/lyrics/";
  CURL* m_curl;  

};

#endif