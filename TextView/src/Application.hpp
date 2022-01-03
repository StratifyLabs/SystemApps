#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <sys/Cli.hpp>
#include <var.hpp>
#include <lvgl.hpp>

#include "model/Model.hpp"

class Application : public ModelAccess {
public:
  static void run(const sys::Cli &cli);

private:

  static void initialize(const sys::Cli &cli);
};

#endif // APPLICATION_HPP
