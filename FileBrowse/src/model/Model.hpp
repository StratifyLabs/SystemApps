//
// Created by Tyler Gilbert on 12/16/21.
//

#ifndef FILEBROWSE_MODEL_HPP
#define FILEBROWSE_MODEL_HPP

#include <lvgl/Runtime.hpp>
#include <lvgl/Theme.hpp>
#include <printer/YamlPrinter.hpp>

#include <design/extras/Form.hpp>
#include <design/ModelScope.hpp>


struct Model {
public:
  static constexpr auto worker_done_message = "Done";

  // all access to the model must be withing a Scope
  class Scope : public design::ModelScope {
  public:
    Scope() : design::ModelScope(Model::instance().model_scope_options) {}
  };

  printer::YamlPrinter printer;
  lvgl::Runtime *runtime = nullptr;
  bool is_project_path_valid = false;
  bool is_theme_updated = false;
  bool is_dark_theme = false;


  lvgl::Theme light_theme;
  lvgl::Theme dark_theme;

private:
  friend Scope;
  friend class ModelAccess;

  design::ModelScope::Construct model_scope_options;

  Model() = default;

  static Model &instance() {
    static Model model;
    return model;
  }
};

class ModelAccess : public api::ExecutionContext {
public:
  static Model &model() {
    // make sure the caller has locked the model
    Model &result = Model::instance();
    API_ASSERT(result.model_scope_options.is_available());
    return Model::instance();
  }
  static printer::Printer &printer() { return model().printer; }

};

#endif // FILEBROWSE_MODEL_HPP
