#include <fs.hpp>

#include "Folder.hpp"

Folder::Folder(Object parent, const Create& options){
  m_object = api()->list_create(parent.object());
  set_name(options.name());
  m_path = options.path();
}


void Folder::initialize(){

  printf("size of event:%d\n", sizeof(lv_event_code_t));
  if( FileSystem().directory_exists(m_path) == false ){
    send_alert(get_parent());
    return;
  }

  //load the path
  const auto list = FileSystem().read_directory(m_path);

  //add items in the director to the list
  for(const auto & item: list){
    printf("adding %s\n", item.cstring());
    api()->list_add_text(object(), item.cstring());
  }
}
