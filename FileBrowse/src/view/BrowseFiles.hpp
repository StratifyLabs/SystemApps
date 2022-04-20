//
// Created by Tyler Gilbert on 12/31/21.
//

#ifndef FILEBROWSE_SRC_VIEW_FILESYSTEM_HPP
#define FILEBROWSE_SRC_VIEW_FILESYSTEM_HPP

#include "../model/Model.hpp"

#include <lvgl/Generic.hpp>

class BrowseFiles : public ModelAccess, public lvgl::ObjectAccess<BrowseFiles> {
public:
  static void setup(lvgl::Generic generic, var::StringView path){
    generic.add(BrowseFiles("BrowseFiles", path));
  }

private:
  BrowseFiles(const char * name, var::StringView path);
  explicit BrowseFiles(lv_obj_t * object){
    m_object = object;
  }

};

#endif // FILEBROWSE_SRC_VIEW_FILESYSTEM_HPP
