//
// Created by Tyler Gilbert on 12/31/21.
//

#include <design.hpp>
#include <lvgl.hpp>

#include "SelectFiles.hpp"

void SelectFiles::setup(lvgl::Generic generic, var::StringView path) {
  static auto file_system_user_data =
    FileSystemCard::Data("fileSystemWindow").set_base_path(path).set_select_file();

  generic.add(FileSystemCard(file_system_user_data).set_radius(0).fill())
    .add_event_callback(EventCode::exited, [](lv_event_t *) {
      Model::Scope ms;
      model().runtime->set_stopped();
    });
}