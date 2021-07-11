#ifndef FOLDER_HPP
#define FOLDER_HPP

#include <lvgl/List.hpp>
#include <var/StackString.hpp>

#include "Model.hpp"

using namespace lv;

class Folder : public lv::ObjectAccess<Folder>, public ModelAccess {
public:
  class Create : public CreateAccess<Create> {
  public:
    Create(const char *name)
        : CreateAccess(name) {}

  private:
    API_AC(Create, var::PathString, path);
  };

  Folder(Object parent, const Create &options);

  static void configure(Folder &folder, void *) { folder.configure(); }


private:
  static constexpr auto title_height = 15_percent;
  static constexpr auto list_height = 84_percent;

  void configure();
  static void configure_list(lv::List & list, void * context){
    reinterpret_cast<Folder*>(context)->configure_list(list);
  }

  void configure_list(lv::List & list);
};

#endif // FOLDER_HPP
