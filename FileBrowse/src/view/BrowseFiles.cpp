//
// Created by Tyler Gilbert on 12/31/21.
//

#include <lvgl.hpp>
#include <design.hpp>

#include "BrowseFiles.hpp"

BrowseFiles::BrowseFiles(const char * name, const var::StringView path) {
  construct_object(name);
  fill();

  static auto file_system_user_data =
    FileSystemCard::Data("fileSystemWindow").set_base_path(path);

  add(FileSystemCard(file_system_user_data).set_radius(0).fill())
    .add_event_callback(EventCode::exited, [](lv_event_t *) {
      Model::Scope ms;
      model().runtime->set_stopped();
    });

}