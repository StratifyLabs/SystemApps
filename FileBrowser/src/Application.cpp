#include <fs.hpp>
#include <lvgl.hpp>

#include "Application.hpp"

void Application::loop(Runtime &rt) {
  rt.set_period(50_milliseconds);

  model().path = "/";

  Container::active_screen().add<Window>(
    lvgl::Window::Create(model().file_browser_window)
      .set_height(20_percent)
      .configure([](Window &window) {
        window.clear_flag(Window::Flags::scrollable)
          .add_button(model().back_button_name, LV_SYMBOL_LEFT, 20_percent)
          .add_title(
            model().window_title_name, model().path,
            [](Label &label) { label.set_left_padding(10); })
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
          .add<TileView>(
            TileView::Create(model().tile_view_name).configure([](TileView &tile_view) {
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
                .add_event_callback(EventCode::all, [](lv_event_t *lv_event) {
                  const Event event(lv_event);
                  // printf("TileViewEvent %s\n",
                  // Event::to_cstring(event.code()));
                });
            }));
      }));

  rt.loop();
}

void Application::add_details(Container &container) {

  model().file_info = FileSystem().get_info(model().path);

  const auto &info = model().file_info;
  if (info.is_file()) {
    model().file_type = "File";
  } else if (info.is_device()) {
    model().file_type = "Device";
  }

  container.add<Table>(
    Table::Create(model().file_details_table_name).configure([](Table &table) {
      const auto width = Container::active_screen().get_width();
      table.set_width(100_percent)
        .set_height(100_percent)
        .set_column_count(2)
        .set_row_count(4)
        .set_column_width(0, width / 2)
        .set_column_width(1, width / 2)
        .set_cell_value(Table::Cell().set_column(0).set_row(0), "Type")
        .set_cell_value(Table::Cell().set_column(0).set_row(1), "Size")
        .set_cell_value(Table::Cell().set_column(0).set_row(2), "Mode")
        .set_cell_value(Table::Cell().set_column(0).set_row(3), "Owner")
        .set_cell_value(Table::Cell().set_column(1).set_row(0), model().file_type)
        .set_cell_value(
          Table::Cell().set_column(1).set_row(1),
          var::NumberString(model().file_info.size()))
        .set_cell_value(
          Table::Cell().set_column(1).set_row(2),
          var::NumberString(model().file_info.permissions().permissions(), "0%o"))
        .set_cell_value(
          Table::Cell().set_column(1).set_row(3),
          model().file_info.owner() ? "user" : "root")
        .add_event_callback(EventCode::draw_part_begin, [](lv_event_t *e) {
          const Event event(e);
          Draw draw(event.parameter<lv_obj_draw_part_dsc_t *>());

          /*If the cells are drawn...*/
          if (draw.part() == Part::items) {
            uint32_t row = draw.id() / 2;

            /*MAke every 2nd row grayish*/
            if ((row % 2) == 0) {
              auto rect = draw.get_rectangle();
              const auto background_color = rect.background_color();
              const auto mix_color =
                Color::get_palette(Palette::grey).mix(background_color, MixRatio::x10);
              rect.set_background_color(mix_color).set_background_opacity(Opacity::cover);
            }
          }
        });
    }));
}

void Application::configure_list(List &list) {
  // load the path
  const auto file_list = FileSystem().read_directory(model().path);

  const auto path_prefix = model().path == "/" ? "" : model().path.string_view();

  list.set_top_padding(0)
    .set_scroll_mode(ScrollBarMode::active)
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
    {
      api::ErrorScope es;
      const auto *symbol = FileSystem().get_info(full_path).is_directory()
                             ? LV_SYMBOL_DIRECTORY
                             : LV_SYMBOL_FILE;
      list.add_button(symbol, item.cstring());
    }
  }
}

void Application::go_forward(const var::StringView selected) {

  model().path =
    model().path == "/" ? model().path.append(selected) : model().path / selected;

  printf("go forward to %s\n", model().path.cstring());

  const auto info = FileSystem().get_info(model().path);
  if (is_error()) {
    printf("failed to get info for %s\n", model().path.cstring());
    printf("Message: %s\n", error().message());
    printf("Error: %d\n", error().error_number());
  }
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

  printf("Go backward %d\n", model().tile_column);

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
