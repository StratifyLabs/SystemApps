#include <fs.hpp>
#include <lvgl.hpp>

#include "Folder.hpp"

Folder::Folder(Object parent, const Create &options) {
  m_object = api()->obj_create(parent.object());
  set_name(options.name());
  m_path = options.path();
}

void Folder::initialize() {

  set_layout(*Api::api()->layout_flex)
      .set_flex_flow(FlexFlow::column)
      .set_top_padding(0)
      .set_border_width(0)
      .set_bottom_padding(0)
      .set_left_padding(0)
      .set_right_padding(0)
      .set_width(100_percent)
      .set_height(100_percent);

  printf("size of event:%d\n", sizeof(lv_event_code_t));
  if (FileSystem().directory_exists(m_path) == false) {
    send_alert(get_parent());
    return;
  }

  add<Label>(Label::Create("FolderTitle")
                 .configure(this, [](Label &label, void *context) {
                   Folder *self = reinterpret_cast<Folder *>(context);

                   label.set_height(15_percent)
                       .set_width(100_percent)
                       .set_text(self->m_path.cstring());
                 }));

  add<lv::List>(lv::List::Create("folderList")
                    .configure(this, [](lv::List &list, void *context) {
                      // load the path
                      Folder *self = reinterpret_cast<Folder *>(context);
                      const auto file_list =
                          FileSystem().read_directory(self->m_path);

                      list.set_top_padding(0)
                          .set_border_width(0)
                          .set_bottom_padding(0)
                          .set_width(100_percent)
                          .set_height(80_percent);

                      // add items in the director to the list
                      for (const auto &item : file_list) {
                        printf("adding %s\n", item.cstring());
                        list.add_button("", item.cstring());
                      }
                    }));
}
