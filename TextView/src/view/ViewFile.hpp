//
// Created by Tyler Gilbert on 12/31/21.
//

#ifndef GUI_FILESYSTEM_HPP
#define GUI_FILESYSTEM_HPP

#include <design/macros.hpp>
#include <lvgl/Generic.hpp>

#include "../model/Model.hpp"


class ViewFile : public ModelAccess {
public:
  static void setup(lvgl::Generic generic, var::StringView path);

private:
  struct Names {
    DESIGN_DECLARE_NAME(file_text_area);
  };

};

#endif // GUI_FILESYSTEM_HPP
