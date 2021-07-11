#include <fs.hpp>
#include <lvgl.hpp>

#include "Application.hpp"

Application::Application() {}

void Application::run() {

  Runtime rt;
  rt.set_period(10_milliseconds).initialize();

  model().path = "/";

  Container::active_screen().add<Window>(
    Window::Create(model().file_browser_window)
      .set_height(20_percent)
      .configure([](Window &window) {
        window.add_button(model().back_button_name, LV_SYMBOL_LEFT, 15_percent)
          .add_title(model().window_title_name, model().path, [](Label & label){
            label.set_left_padding(10);
          })
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
                           .configure([](TileView &tile_view) {
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


  rt.loop();
}

void Application::add_details(Container &container) {

  container.add<Label>(Label::Create(nullptr).configure([](Label & label){
    label.set_text("File Details for " | model().path);
  }));

}

void Application::configure_list(List& list) {
  // load the path
  const auto file_list = FileSystem().read_directory(model().path);

  const auto path_prefix = model().path == "/" ? "" : model().path.string_view();

  list.set_top_padding(0)
    .set_border_width(0)
    .set_bottom_padding(0)
    .set_width(100_percent)
    .set_height(list_height)
    .add_flag(List::Flags::event_bubble)
    .add_event_callback(EventCode::clicked, [](lv_event_t *lv_event) {
      const Event event(lv_event);
      printf("Clicked\n");
      if (event.target().name() != model().entry_list_name) {

        const char *text =
          event.current_target().cast<List>()->get_button_text(event.target());

        go_forward(text);
      }
    });

  // add items in the director to the list
  for (const auto &item : file_list) {
    auto full_path = path_prefix / item;
    const auto *symbol = FileSystem().get_info(full_path).is_directory()
                           ? LV_SYMBOL_DIRECTORY
                           : LV_SYMBOL_FILE;
    list.add_button(symbol, item.cstring());
  }
}

void Application::go_forward(const var::StringView selected) {

  model().path =
    model().path == "/" ? model().path.append(selected) : model().path / selected;

  printf("go forward to %s\n", model().path.cstring());

  const auto info = FileSystem().get_info(model().path);
  auto screen = Container::active_screen();

  screen.find(model().window_title_name).cast<Label>()->set_text(model().path);

  model().tile_column++;
  if (info.is_directory()) {
    screen.find(model().tile_view_name)
      .cast<TileView>()
      ->add_tile(
        "DynamicTile", TileView::Location().set_column(model().tile_column),
        [](Container &container) {
          container.add<List>(List::Create(nullptr).configure(configure_list));
        })
      .update_layout()
      .set_tile(TileView::Location().set_column(model().tile_column));

  } else {
    screen.find(model().tile_view_name)
      .cast<TileView>()
      ->add_tile(
        "DynamicTile", TileView::Location().set_column(model().tile_column), add_details)
      .update_layout()
      .set_tile(TileView::Location().set_column(model().tile_column));
  }
}

void Application::go_backward() {

  if (model().tile_column == 0) {
    return;
  }

  auto screen = Container::active_screen();

  model().tile_column--;
  auto tile_view = screen.find(model().tile_view_name);
  auto to_be_removed = tile_view.cast<TileView>()->get_active_tile();

  tile_view.cast<TileView>()->set_tile(
    TileView::Location().set_column(model().tile_column));

  to_be_removed.async_remove();

  model().path = model().path == "/"
                   ? model().path
                   : var::PathString(Path::parent_directory(model().path));

  if (model().path.is_empty()) {
    model().path = "/";
  }

  screen.find(model().window_title_name).cast<Label>()->set_text(model().path);
}
