
#include <fs.hpp>
#include <lvgl.hpp>

#include "Application.hpp"

Application::Application(const sys::Cli &cli, Runtime &rt) {
  rt.set_period(50_milliseconds);
  const auto path_argument = cli.get_option("path");

  const auto path = path_argument.is_empty() ? "/" : path_argument;

  File file(path);
  var::Array<u8, 2048> file_buffer;
  static auto user_data =
    FileTextArea::Data("fileSystemWindow", View(file_buffer)).set_file(&file);

  API_PRINTF_TRACE_LINE();
  model().runtime = &rt;

  screen()
    .add(FileTextArea(user_data).fill().set_text_font(Font::find("sourcecode", 20)))
    .add_event_callback(

      // confirm with a model message box before exiting

      EventCode::exited, [](lv_event_t *) { model().runtime->set_stopped(); });

  rt.loop();
}
