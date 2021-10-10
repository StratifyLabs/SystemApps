
#include <fs.hpp>
#include <lvgl.hpp>

#include "Application.hpp"

Application::Application(const sys::Cli &cli, Runtime &rt) {
  rt.set_period(50_milliseconds);
  const auto path_argument = cli.get_option("path");

  const auto path = path_argument.is_empty() ? "/" : path_argument;

  static auto file_system_user_data =
    FileSystemWindow::FileSystemData("fileSystemWindow").set_base_path(path);

  model().runtime = &rt;

  screen()
    .add(FileSystemWindow(
           file_system_user_data,
           FileSystemWindow::Construct().set_header_height(20_percent))
           .configure([](FileSystemWindow &fs_window) {
             fs_window.set_width(100_percent).set_height(100_percent);
           }))
    .add_event_callback(
      EventCode::exited, [](lv_event_t *) { model().runtime->set_stopped(); });

  rt.loop();
}
