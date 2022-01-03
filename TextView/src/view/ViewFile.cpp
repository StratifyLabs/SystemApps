//
// Created by Tyler Gilbert on 12/31/21.
//

#include <design.hpp>
#include <design/extras/FileTextArea.hpp>
#include <lvgl.hpp>
#include <var.hpp>

#include "ViewFile.hpp"

void ViewFile::setup(lvgl::Generic generic, var::StringView path) {
  static var::Array<u8, 8192> buffer;
  static fs::File file(path);
  static auto text_area_data =
    FileTextArea::Data(Names::file_text_area, View(buffer)).set_file(&file);

  generic.add(Container().fill().add(FileTextArea(text_area_data).set_border_width(0).fill()));
}