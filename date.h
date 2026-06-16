#ifndef DATE_H_
#define DATE_H_

/* Date class - simple class to handle dates, sort them comfortably */
#include <string>
#include <cctype>
#include <fstream>

class Date {
public:
  Date(const std::string& date, const std::string& format = "yyyy-mm-dd") : m_date(date), m_format(format){
    for(int i = 0; i < m_format.size();i++)
      m_format[i] = tolower(m_format[i]);
    parse_date();
  };
  Date(const Date& other){
    this->m_date = other.m_date;
    this->m_format = other.m_format;
    this->m_year = other.m_year;
    this->m_day = other.m_day;
    this->m_month = other.m_month;

  };
  ~Date() {};
  Date operator=(const Date& other) {
    this->m_date = other.m_date;
    this->m_format = other.m_format;
    this->m_year = other.m_year;
    this->m_day = other.m_day;
    this->m_month = other.m_month;

    return *this;
  }

  std::string get_date() const {return m_date;};
  std::string get_year() const { return m_year; };
  std::string get_month() const { return m_month; };
  std::string get_day() const { return m_day; };

  void operator++() {
    int year = stoi(this->m_year);
    int month = stoi(this->m_month);
    int day = stoi(this->m_day);

    day++;
    int days_in_month = 0;
    switch(month){ // jan 1 3 5 8 7 10 12
      case 1:
      case 3:
      case 5:
      case 7:
      case 8:
      case 10:
      case 12:
        days_in_month = 31;
        break;
  
      case 4:
      case 6:
      case 9:
      case 11:
        days_in_month = 30;
        break;
      
      case 2:
        if(year%4 == 0)
          days_in_month = 29;
        else days_in_month = 28;
        break;
    default:
      std::cerr << "unknown month" << std::endl;
    }

    if(day > days_in_month){  
      month += day%days_in_month;
      day /= days_in_month;
    }

    if(month > 12) {
      year += month % 12; 
      month /=12;
    }

    if(day < 10)
      m_day = "0" + std::to_string(day);
    else 
      m_day = std::to_string(day);

    if(month < 10)
      m_month = "0" + std::to_string(month);
    else
      m_month = std::to_string(month);

    m_year = std::to_string(year);

    m_date = m_year +'-'+ m_month +'-'+ m_day;
  }
  void operator--() {
    int year = stoi(this->m_year);
    int month = stoi(this->m_month);
    int day = stoi(this->m_day);

    day--;
    int days_in_month = 0;
    switch(month){ // jan 1 3 5 8 7 10 12
      case 1:
      case 3:
      case 5:
      case 7:
      case 8:
      case 10:
      case 12:
        days_in_month = 31;
        break;
  
      case 4:
      case 6:
      case 9:
      case 11:
        days_in_month = 30;
        break;
      
      case 2:
        if(year%4 == 0)
          days_in_month = 29;
        else days_in_month = 28;
        break;
    default:
      std::cerr << "unknown month" << std::endl;
    }

    if(day < 1){  
      day= days_in_month;
      month--;
    }

    if(month < 1) {
      year--; 
      month = 12;
    }

    if(day < 10)
      m_day = "0" + std::to_string(day);
    else 
      m_day = std::to_string(day);

    if(month < 10)
      m_month = "0" + std::to_string(month);
    else
      m_month = std::to_string(month);

    m_year = std::to_string(year);

    m_date = m_year +'-'+ m_month +'-'+ m_day;
  }

  bool operator>(const Date& other) const {
    int first_year = stoi(this->m_year),
        second_year = stoi(other.m_year);
    int first_month = stoi(this->m_month),
        second_month = stoi(other.m_month);
    int first_day = stoi(this->m_day),
        second_day = stoi(other.m_day);
    

    if(first_year == second_year) {
      
      if(first_month == second_month)
        return (first_day > second_day);
      else
        return (first_month > second_month);

    } else return (first_year > second_year);
  } 
  bool operator==(const Date& other) const {
    return (this->m_year == other.m_year
          && this->m_month == other.m_month
          && this->m_day == other.m_day);
  }

  bool operator<(const Date& other) const {
    return (!(*this > other) && !(*this == other)); // not equal and not bigger
  } 
  bool operator<=(const Date& other) const {
    return (*this < other) || (*this == other); // equal or less
  } 

  void next_month() {
    int year = stoi(this->m_year);
    int month = stoi(this->m_month);
    int day = stoi(this->m_day);

    month++;
    day = 1;

    if(month > 12) {
      year += month % 12; 
      month /=12;
    }

    if(day < 10)
      m_day = "0" + std::to_string(day);
    else 
      m_day = std::to_string(day);

    if(month < 10)
      m_month = "0" + std::to_string(month);
    else
      m_month = std::to_string(month);

    m_year = std::to_string(year);

    m_date = m_year +'-'+ m_month +'-'+ m_day;
  }
  
  void prev_month() {
    int year = stoi(this->m_year);
    int month = stoi(this->m_month);
    int day = stoi(this->m_day);

    month--;
    
    if(month < 1) {
      year -= 1; 
      month = 12;
    }

    int days_in_month = 0;
    switch(month){ // jan 1 3 5 8 7 10 12
      case 1:
      case 3:
      case 5:
      case 7:
      case 8:
      case 10:
      case 12:
        days_in_month = 31;
        break;
  
      case 4:
      case 6:
      case 9:
      case 11:
        days_in_month = 30;
        break;
      
      case 2:
        if(year%4 == 0)
          days_in_month = 29;
        else days_in_month = 28;
        break;
    default:
      std::cerr << "unknown month" << std::endl;
    }

    day = days_in_month;

    if(day < 10)
      m_day = "0" + std::to_string(day);
    else 
      m_day = std::to_string(day);

    if(month < 10)
      m_month = "0" + std::to_string(month);
    else
      m_month = std::to_string(month);

    m_year = std::to_string(year);

    m_date = m_year +'-'+ m_month +'-'+ m_day;
  }
private:
  void parse_date() {
    for(int i =0; i < m_date.size();i++){
      if(i < m_format.size()) {
        if(m_format[i] == 'y')
          m_year += m_date[i];
        else if(m_format[i] == 'm')
          m_month += m_date[i];
        else if(m_format[i] == 'd')
          m_day += m_date[i];
      }
    }
  };
  std::string m_format;
  std::string m_date;
  std::string m_year;
  std::string m_month;
  std::string m_day;
};


#endif