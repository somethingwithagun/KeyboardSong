#include "plotwidget.h"
#include <SFML/Graphics/PrimitiveType.hpp>
template<class T>
PlotWidget<T>::PlotWidget(){
  m_current_mx = INT_MIN;
  m_current_mn = INT_MAX;

  m_brush.color = sf::Color::Black;
  m_brush.style = sf::PrimitiveType::LineStrip;
  m_brush.size = 3;
  m_current_dots = 0;
}

template <class T>
PlotWidget<T>::PlotWidget(const sf::Vector2f& size) {
  setSize(size);

  PlotWidget();
}

template<class T>
PlotWidget<T>::~PlotWidget() {
  while(!m_objects.empty()) {
    delete m_objects.back();
    m_objects.pop_back();
  }
  while(!m_level_lines.empty()) {
    delete m_level_lines.back();
    m_level_lines.pop_back();
  }
  while(!m_points.empty()) {
    delete m_points.back();
    m_points.pop_back();
  }
  if(m_background != nullptr)
    delete m_background;
  if(m_is_font_loaded)
    delete m_font;
}
template<class T>
void PlotWidget<T>::processData(const std::vector<std::pair<T,float>>& passed_data) {
  if(passed_data.empty()) {
    updateData();
    return;
  }
  reset();

  for(int i = 0; i < passed_data.size(); i++)
    apply(passed_data[i].first, passed_data[i].second);

  updateData();
}
template <class T>
bool PlotWidget<T>::loadFromFile(const std::string& path) {
  // assert(0 && "not implemented");
  bool success = true;
  std::ifstream in(path);
  if(in.is_open()) {
    
    std::string time;
    float rating;
    
    while(in >> rating >> time)
      apply(time, rating);
    
    in.close();
  } else success = false;
  updateData();
  return success;
}
template <>
bool PlotWidget<float>::loadFromFile(const std::string& path) {
  // assert(0 && "not implemented");
  bool success = true;
  std::ifstream in(path);
  if(in.is_open()) {
    
    float time;
    float rating;
    
    while(in >> rating >> time)
      apply(time, rating);
    
    in.close();
  } else success = false;
  updateData();
  return success;
}
template <class T>
void PlotWidget<T>::reset() {
  data.clear();
  m_objects.clear();
  m_level_lines.clear();
  m_points.clear();
  m_lines_y.clear();
  updateData();
}

template<class T>
int PlotWidget<T>::find_level_exceed(const int& y) {
  int level_exceed = y;
  while (level_exceed >= m_level_difference) level_exceed -= m_level_difference;

  return level_exceed;
}

template<class T>
void PlotWidget<T>::updateData() {
  m_brush.color = sf::Color::White;

  if(!m_background){
    m_background = create_rect(y_labels_offset, -x_labels_offset, m_bg_width, m_bg_height,m_brush.color);
    setRelativePosition(m_background);
  }

  rescale();
}

template <class T>
void PlotWidget<T>::rescale() {

  // Clear arrays with any objects
  m_points.clear();
  m_lines_y.clear();
  m_level_lines.clear();
  m_objects.clear();

  if(data.empty()) {
    // leave 0(zero)
    m_brush.color = sf::Color(58, 61, 59,90);

    int y = m_bg_height - x_labels_offset;
    
    m_lines_y.push_back(0); //not in pixels
    
    m_level_lines.push_back(create_line(y_labels_offset,y,
          m_bg_width + y_labels_offset,y, m_brush.size, m_brush.color));
    setRelativePosition(m_level_lines.back());

    m_brush.color = sf::Color::White;

    // draw label
    m_objects.push_back(
      create_text(std::to_string(0),
        0, y, 16, m_font, m_brush.color)
    );
    setRelativePosition(m_objects.back());
  }

  if(data.size() > 1)
    m_point_distance = m_bg_width/(data.size()-1);

  int mn = m_current_mn - find_level_exceed(m_current_mn);
  int lines_amount = (m_current_mx-m_current_mn)/m_level_difference + 2;
  if(data.empty()) lines_amount = 1;
  int level_height = m_bg_height/lines_amount;
  
  int i = 0;
  for(i = 0; i < lines_amount;i++) {
    m_brush.color = sf::Color(58, 61, 59,90);

    int y = m_bg_height -(i*level_height) - x_labels_offset;
    
    m_lines_y.push_back(i*m_level_difference+mn); //not in pixels
    
    m_level_lines.push_back(create_line(y_labels_offset,y,
          m_bg_width + y_labels_offset,y, m_brush.size, m_brush.color));
    setRelativePosition(m_level_lines.back());

    m_brush.color = sf::Color::White;

    // draw labels
    m_objects.push_back(
      create_text(std::to_string(m_lines_y[i]),
        0, y, 16, m_font, m_brush.color)
    );
    setRelativePosition(m_objects.back());
  }

  int label_x = m_offset_x + y_labels_offset;
  int brush_size = m_brush.size;
  m_brush.size = 1;
  
  for(i = 0; i<data.size();i++) {

    /* * * Draw Label by X * * */

    m_brush.color = sf::Color::White;
    sf::String label = to_text(data[i].first);
    m_objects.push_back(create_text(label,label_x,m_bg_height+8, 16, m_font, m_brush.color));
    setRelativePosition(m_objects.back());
    label_x += m_point_distance;

    /* * * Draw Points * * */

    int level_exceed = find_level_exceed(data[i].second), line_index = 0;

    auto line_iter = std::find(m_lines_y.begin(),m_lines_y.end(),data[i].second-level_exceed);    
    if(line_iter != m_lines_y.end())
      line_index = line_iter - m_lines_y.begin();

    int x,y;
    y = (m_bg_height - line_index*level_height - x_labels_offset) - (level_exceed*level_height/m_level_difference); // some maths
    x = i*m_point_distance;

    m_brush.color = sf::Color::Red;
    
    m_points.push_back(create_ellipse(x - m_point_radius + m_offset_x + y_labels_offset, 
         y - m_point_radius,m_point_radius*2,m_point_radius*2, m_brush.color));
    setRelativePosition(m_points.back());
    m_points_y[data[i].second] = y;

    if(i > 0) { // if it's not the first point we should connect two points with a line
      m_brush.color = sf::Color::Black;
      int prev_point_x = (i-1) * m_point_distance;

      m_objects.push_back(create_line(
        prev_point_x + m_offset_x + y_labels_offset, // x1 position in pixels
        m_points_y[data[i-1].second], // y1 position in pixels
        x + m_offset_x + y_labels_offset, // x2 position in pixels
        m_points_y[data[i].second], // y2 position in pixels
        m_brush.size, // thickness
        m_brush.color // color
      ));

      setRelativePosition(m_objects.back());

      m_brush.color = sf::Color::Red;
    }
  }
  m_brush.size = brush_size;
}
template <class T>
void PlotWidget<T>::apply(T x, float y){
  data.push_back(std::pair<T, float>(x,y));

  m_current_mx = std::max(int(y),m_current_mx);
  m_current_mn = std::min(int(y),m_current_mn);

  std::sort(data.begin(),data.end(), [](const std::pair<T,int>& first, const std::pair<T,int>& second) {
    return (first.first < second.first);
  });

  rescale();
}
template <class T>
void PlotWidget<T>::draw(sf::RenderTarget& wnd, sf::RenderStates states) const {
  wnd.draw(*m_background);
  for (int i = 0; i < m_objects.size(); i++)
      wnd.draw(*dynamic_cast<sf::Drawable*>(m_objects[i]));
  for (int i = 0; i < m_level_lines.size(); i++)
      wnd.draw(*m_level_lines[i]);
  for (int i = 0; i < m_points.size(); i++)
      wnd.draw(*m_points[i]);
}

template<class T>
void PlotWidget<T>::setSize(const sf::Vector2f& size) {
  m_size = size;
  m_bg_width  = size.x - y_labels_offset;
  m_bg_height = size.y - x_labels_offset;
}

template<class T>
const sf::Vector2f& PlotWidget<T>::getSize() const {
  return m_size;
}

template<class T>
void PlotWidget<T>::setFont(sf::Font* font) {
  m_font = font;
  m_is_font_loaded = false;
}
