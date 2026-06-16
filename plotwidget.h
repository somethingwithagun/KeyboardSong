#ifndef PLOT_H_
#define PLOT_H_
#define DEBUG_


#include <iostream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <climits>

#include "drawfuncs.h"
#include "constants.h"

/*
  *** PlotWidget class declaration ***

 * This class allows to use plots in c++.
 * Template accepts X and Y axis types (only Y implemented by 03.08.24).
 * To load some data you can pass data using processData(const std::vector<T,U>&) function.
 * Scaling options are not very flexible,
      by now you can only change font and size of the widget itself.
 * to draw window 

  Y VALUE TO RELATIVE Y PIXELS CONVERTATION FORMULAS: 
    * index * m_level_difference -- line relative height in value
    * m_bg_height - index * level_height - x_labels_offset -- line relative height in pixels 


    Plot should look like this

    ^ Y, value
    |
    |
    |
    0--------> X, value 

*/

template<class T>
class PlotWidget : public sf::Transformable, public sf::Drawable{
public:
  explicit PlotWidget();
  explicit PlotWidget(const sf::Vector2f& size);
  ~PlotWidget();
  
  
  void processData(const std::vector<std::pair<T,float>>& passed_data);

  /// @brief Allows you to pass data for plot straight from file
  /// @param path Path to the source file
  /// @return Result of opening and reading the source file
  bool loadFromFile(const std::string& path);

  void reset();

  void apply(T x, float y);

  virtual void draw(sf::RenderTarget& wnd, sf::RenderStates states) const;

  void updateData();
  
  // setters
  void setSize(const sf::Vector2f& size);
  const sf::Vector2f& getSize() const;

  void setFont(sf::Font* font);

  bool loadFont(const std::string& path) {
    if(!m_font)
      m_font = new sf::Font;
    return m_is_font_loaded = m_font->openFromFile(path);
  }
  

private:

  /// @brief If the value is not belong to the levels line then it should be somewhere in between two lines; 
  // this block of code is calculating the points y including its offset
  /// @param y Position of the point
  /// @return The exceeding of the y over the nearest level line height
  int find_level_exceed(const int& y);

  void rescale();

  void setRelativePosition(sf::Transformable* object) {
    sf::Vector2f pos = this->getPosition();
    sf::Vector2f origin = this->getOrigin();
    object->move(sf::Vector2f(pos.x - origin.x - y_labels_offset/2, pos.y - origin.y));
  }

  struct {
    sf::Color color;
    sf::PrimitiveType style;
    int size;
  } m_brush; 

  std::string to_text(std::string str) {return str;}

  std::string to_text(float num, int accuracy = 1) {
    std::stringstream ss;
    ss << std::fixed
      << std::setprecision(std::log(accuracy) / std::log(10)) /* base-10 logarithm*/
      << num;
    return ss.str();
  }

  int m_bg_height;
  int m_bg_width;
  
  int m_current_mx;
  int m_current_mn;

  std::vector<std::pair<T,int>> data;

  const int m_level_difference = 1000;

  // this might be a bit confusing
  const int y_labels_offset = 30; // this is the offset for the labels that go along the Y axis. So it moves graph along X axis right
  const int x_labels_offset = 30; // this is the offset for the labels that go along the X axis. So it moves graph along Y axis up
  
  int m_max_dots;
  int m_current_dots;

  const int m_offset_x = 0; // left margin relatively to the background(that means that labels offsets are included too)
  int m_point_distance = 90; // distance between the dots
  const int m_point_radius = 3; // radius of the dot

  sf::Vector2f m_size;

  sf::Font* m_font = nullptr;
  bool m_is_font_loaded = false;

  std::vector <sf::Transformable*> m_objects;
  
  std::vector <sf::RectangleShape*> m_level_lines;
  std::vector <int> m_lines_y;

  std::vector <sf::CircleShape*> m_points;
  std::unordered_map <int,int> m_points_y; // value, position y

  sf::RectangleShape* m_background = nullptr;
};

template class PlotWidget<sf::String>;
template class PlotWidget<float>;

#endif
