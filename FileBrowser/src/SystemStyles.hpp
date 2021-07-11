#ifndef SYSTEMSTYLES_HPP
#define SYSTEMSTYLES_HPP

#include <lvgl/Style.hpp>

class SystemStyles {
public:
  SystemStyles();
  const lvgl::Style &no_padding() const { return m_no_padding; }

  static const SystemStyles * styles(){
    return m_styles;
  }

private:
  static const SystemStyles * m_styles;
  lvgl::Style m_no_padding;


};

#endif // SYSTEMSTYLES_HPP
