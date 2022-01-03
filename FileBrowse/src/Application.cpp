
#if defined __link
#include "designlab/fonts/fonts.h"
#include "designlab/themes/themes.h"
#endif

#include "Application.hpp"
#include "view/BrowseFiles.hpp"

void Application::run(const sys::Cli &cli) {
  initialize(cli);
  auto *runtime = []() {
    Model::Scope ms;
    auto *result = model().runtime;
    Display(result->display()).set_theme(model().dark_theme);
    return model().runtime;
  }();

  const auto path_argument = cli.get_option("path");
  const auto path = path_argument.is_empty() ? "/" : path_argument;

  BrowseFiles::setup(screen(), path);

  runtime->loop();
}

void Application::initialize(const sys::Cli &cli) {
#if defined __link
  api::catch_segmentation_fault();
  // on `link` (desktop) the application needs to provide
  // the fonts, themes, etc
  static lvgl::Runtime runtime(
    "File Browse", window::Point(), window::Size(320 * 6, 240 * 6),
    window::Window::Flags::shown | window::Window::Flags::highdpi
    /*| window::Window::Flags::resizeable*/);

  runtime.window().set_minimum_size(window::Size(480, 360));

  // make the fonts available to `Font::find()`
  fonts_initialize();

  {
    Model::Scope model_scope;
    model().runtime = &runtime;
    model().light_theme =
      Theme(lvgl_small_light_theme_initialize(runtime.display(), nullptr));
    model().dark_theme =
      Theme(lvgl_small_dark_theme_initialize(runtime.display(), nullptr));
    model().is_dark_theme = false;
  }
#else
  // on Stratify OS the system provides
  // the fonts, themes, display size, etc
  static lvgl::Runtime runtime;
  {
    Model::Scope model_scope;

    // grab the light and dark themes for the model

    model().runtime = &runtime;
  }
#endif
}
