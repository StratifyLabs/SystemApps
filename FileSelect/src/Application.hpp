#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <sys/Cli.hpp>
#include <var.hpp>
#include <lvgl.hpp>

class Application : public api::ExecutionContext {
public:
  Application(const sys::Cli &cli, Runtime &rt);

private:
  static constexpr auto back_button_name = "BackButton";
  static constexpr auto entry_list_name = "EntryList";
  static constexpr auto tile_view_name = "TileView";
  static constexpr auto file_browser_window = "FileBrowser";
  static constexpr auto window_title_name = "WindowTitle";
  static constexpr auto file_details_table_name = "FileDetails";

  struct Model {
  public:
    API_SINGLETON(Model);
    enum class FolderStatus { none, ready, back_requested };
    Runtime * runtime = nullptr;

    var::PathString original_path;
    var::GeneralString error;
    var::PathString path;
    FolderStatus folder_status = FolderStatus::ready;

  private:
    static Model *m_model;
  };

  class TileData : public UserDataAccess<TileData> {
  public:
    TileData(const char *path) : UserDataBase(""), m_path_name(path) {}

  private:
    API_AC(TileData, PathString, path_name);
  };

  static Model &model() { return Model::instance(); }

  static constexpr auto title_height = 15_percent;
  static constexpr auto list_height = 84_percent;

  static void configure_list(Container &list);
  static void configure_details(Container container);

  static PathString get_next_path(const char *entry) {
    if (model().path == "/") {
      return PathString(model().path).append(entry);
    }
    return model().path / entry;
  }
};

#endif // APPLICATION_HPP
