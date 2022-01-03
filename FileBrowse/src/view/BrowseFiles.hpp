//
// Created by Tyler Gilbert on 12/31/21.
//

#ifndef GUI_FILESYSTEM_HPP
#define GUI_FILESYSTEM_HPP

#include "../model/Model.hpp"

#include <lvgl/Generic.hpp>

class BrowseFiles : public ModelAccess {
public:
  static void setup(lvgl::Generic generic, var::StringView path);

};

#endif // GUI_FILESYSTEM_HPP
