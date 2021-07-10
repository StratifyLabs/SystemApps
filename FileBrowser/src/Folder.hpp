#ifndef FOLDER_HPP
#define FOLDER_HPP

#include <var/StackString.hpp>
#include <lvgl/List.hpp>

class Folder : public lv::ObjectAccess<Folder>
{
public:
  class Create : public CreateAccess<Create> {
  public:
    Create(const char * name) : CreateAccess(name){}
  private:
    API_AC(Create, var::PathString, path);
  };

  Folder(Object parent, const Create& options);

  static void configure(Folder & folder, void*){
    folder.initialize();
  }

private:
  var::PathString m_path;


  void initialize();
};

#endif // FOLDER_HPP
