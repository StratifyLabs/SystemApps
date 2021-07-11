#include <fs.hpp>
#include <lvgl.hpp>

#include "Folder.hpp"
#include "SystemStyles.hpp"
#include "Model.hpp"

Folder::Folder(Object parent, const Create &options) : ObjectAccess(object_type()) {
  m_object = api()->win_create(parent.object(), 20_percent);
  set_name(options.name());
  model().path = options.path();
}

void Folder::configure() {
  if (FileSystem().directory_exists(model().path) == false) {
    model().error = "path not found: " | model().path.string_view();
    return;
  }

  auto * window = reinterpret_cast<Window*>(this);

  window->add_button(model().back_button_name, LV_SYMBOL_LEFT, 15_percent)
    .add_flag(Flags::event_bubble)
    .add_title("title", (" " & model().path).cstring())
    .set_width(100_percent)
    .set_height(100_percent)
    .add<lv::List>(
      lv::List::Create(model().entry_list_name).configure(configure_list));

  window->get_header().add_flag(Flags::event_bubble);
  window->get_content().add_flag(Flags::event_bubble);
}

void Folder::configure_list(lv::List &list) {
  // load the path
  const auto file_list = FileSystem().read_directory(model().path);

  list.set_top_padding(0)
    .set_border_width(0)
    .set_bottom_padding(0)
    .set_width(100_percent)
    .set_height(list_height)
    .add_flag(Flags::event_bubble);

  // add items in the director to the list
  for (const auto &item : file_list) {
    printf("adding %s\n", item.cstring());
    list.add_button("", item.cstring());
  }
}
