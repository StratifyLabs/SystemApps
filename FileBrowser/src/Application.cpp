#include "Application.hpp"

#include <lvgl.hpp>

#include "Folder.hpp"

Application::Application() {}

void Application::run() {

  constexpr auto top_tile_view = "TopView";

  auto screen = Container::active_screen();

  screen.add<TileView>(
      TileView::Create(top_tile_view).configure([](TileView &obj, void *) {
        obj.add_tile(
               TileView::Location().set_column(0).set_row(0).set_direction(
                   Direction::right | Direction::bottom),
               [](Container &obj) {
                 obj.add<Folder>(Folder::Create("folder").set_path("/").configure(Folder::configure));
               })
            .add_tile(
                TileView::Location().set_column(0).set_row(1).set_direction(
                    Direction::left),
                [](Container &obj) {
                  obj.add<Label>(Label::Create("label").configure(
                      [](Label &obj, void *) { obj.set_text("0,1"); }));
                })
            .add_tile(
                TileView::Location().set_column(1).set_row(0).set_direction(
                    Direction::top),
                [](Container &obj) {
                  obj.add<Label>(Label::Create("label").configure(
                      [](Label &obj, void *) { obj.set_text("1,0"); }));
                });
      }));

  while (1) {
    wait(100_milliseconds);
  }
}
