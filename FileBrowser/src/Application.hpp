#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <lvgl.hpp>

#include "SystemStyles.hpp"
#include "Model.hpp"

using namespace lvgl;

class Application : public ModelAccess {
public:
  Application();

  void run();

private:
  static constexpr auto title_height = 15_percent;
  static constexpr auto list_height = 84_percent;

  SystemStyles m_system_styles;
  Model m_model;

  static void configure_list(lvgl::List & list);
  static void add_details(Container & container);

  static void go_forward(const var::StringView selected);
  static void go_backward();

};



#endif // APPLICATION_HPP
