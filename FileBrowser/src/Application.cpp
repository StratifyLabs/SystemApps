#include <fs.hpp>
#include <lvgl.hpp>

#include "Application.hpp"
#include "Folder.hpp"

Application::Application() {}

void Application::run() {

  model().path = "/";

  Container::active_screen().add<Window>(
    Window::Create(model().file_browser_window)
      .set_height(20_percent)
      .configure([](Window &window, void *) {
        window.add_button(model().back_button_name, LV_SYMBOL_LEFT, 15_percent)
          .add_title(model().window_title_name, (" " & model().path).cstring())
          .set_width(100_percent)
          .set_height(100_percent)
          .add_event_callback(
            EventCode::pressed,
            [](lv_event_t *lv_event) {
              const Event event(lv_event);
              if (event.target().name() == model().back_button_name) {
                printf("Back pressed\n");
                go_backward();
              }
            })
          .add<TileView>(TileView::Create(model().tile_view_name)
                           .configure([](TileView &tile_view, void *) {
                             tile_view.set_width(100_percent)
                               .set_height(100_percent)
                               .add_tile(
                                 "DynamicTile", TileView::Location().set_column(0),
                                 [](Container &container) {
                                   printf(
                                     "adding first tile %s:%p\n", model().path.cstring(),
                                     container.object());
                                   container.add<List>(
                                     List::Create(model().entry_list_name | model().path)
                                       .configure(configure_list));
                                 })
                               .add_event_callback(
                                 EventCode::all, [](lv_event_t *lv_event) {
                                   const Event event(lv_event);
                                   // printf("TileViewEvent %s\n",
                                   // Event::to_cstring(event.code()));
                                 });
                           }));
      }));

  while (1) {
    wait(100_milliseconds);
  }
}

void Application::configure_list(lv::List &list, void *) {
  // load the path
  const auto file_list = FileSystem().read_directory(model().path);

  list.set_top_padding(0)
    .set_border_width(0)
    .set_bottom_padding(0)
    .set_width(100_percent)
    .set_height(list_height)
    .add_flag(List::Flags::event_bubble)
    .add_event_callback(EventCode::clicked, [](lv_event_t *lv_event) {
      const Event event(lv_event);
      if (event.target().name() != model().entry_list_name) {
        const char *text =
          event.current_target().cast<List>()->get_button_text(event.target());
        go_forward(text);
      }
    });

  // add items in the director to the list
  for (const auto &item : file_list) {
    list.add_button("", item.cstring());
  }
}

void Application::go_forward(const var::StringView selected) {

  model().path =
    model().path == "/" ? model().path.append(selected) : model().path / selected;

  printf("Go to path %s\n", model().path.cstring());

  const auto info = FileSystem().get_info(model().path);
  auto screen = Container::active_screen();

  screen.find(model().window_title_name).cast<Label>()->set_text(" " & model().path);

  model().tile_column++;
  if (info.is_directory()) {
    screen.find(model().tile_view_name)
      .cast<TileView>()
      ->add_tile(
        "DynamicTile", TileView::Location().set_column(model().tile_column),
        [](Container &container) {
          printf(
            "adding another tile %s:%p\n", model().path.cstring(), container.object());
          container.add<List>(List::Create(model().entry_list_name | model().path)
                                .configure(configure_list));
        })
      .update_layout()
      .set_tile(TileView::Location().set_column(model().tile_column));

  } else {
    printf("not a directory\n");
  }
}

void Application::go_backward() {

  if (model().tile_column == 0) {
    printf("nothing to do\n");
    return;
  }

  auto screen = Container::active_screen();

  model().tile_column--;
  printf("set column %d\n", model().tile_column);

  auto tile_view = screen.find(model().tile_view_name);
  auto to_be_removed = tile_view.cast<TileView>()->get_active_tile();

  tile_view.cast<TileView>()->set_tile(
    TileView::Location().set_column(model().tile_column));

  printf("delete tile %s:%p\n", to_be_removed.name(), to_be_removed.object());

  to_be_removed.async_remove();

  model().path = model().path == "/"
                   ? model().path
                   : var::PathString(Path::parent_directory(model().path));

  if (model().path.is_empty()) {
    model().path = "/";
  }

  screen.find(model().window_title_name).cast<Label>()->set_text(" " & model().path);
}
