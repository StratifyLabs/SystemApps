//
// Created by Tyler Gilbert on 12/31/21.
//

#include <lvgl.hpp>
#include <design.hpp>

#include "BrowseFiles.hpp"

void BrowseFiles::setup(lvgl::Generic generic, var::StringView path) {
  static auto file_system_user_data =
    FileSystemCard::Data("fileSystemWindow").set_base_path(path);

  generic.add(FileSystemCard(file_system_user_data).set_radius(0).fill())
    .add_event_callback(EventCode::exited, [](lv_event_t *) {
      Model::Scope ms;
      model().runtime->set_stopped();
    });
}