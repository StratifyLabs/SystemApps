#ifndef MODEL_HPP
#define MODEL_HPP

#include <var/Array.hpp>
#include <var/StackString.hpp>
#include <fs/File.hpp>

struct Model {
public:
  enum class FolderStatus { none, ready, back_requested };

  Model();

  static Model &model() { return *m_model; }

  static constexpr auto back_button_name = "BackButton";
  static constexpr auto entry_list_name = "EntryList";
  static constexpr auto tile_view_name = "TileView";
  static constexpr auto file_browser_window = "FileBrowser";
  static constexpr auto window_title_name = "WindowTitle";
  static constexpr auto file_details_table_name = "FileDetails";

  var::GeneralString error;
  var::PathString path;
  FolderStatus folder_status = FolderStatus::ready;
  u32 tile_column = 0;
  fs::FileInfo file_info;
  var::IdString file_type;

private:
  static Model *m_model;
};

#endif // MODEL_HPP
