// COPYING: Copyright 2011-2021 Stratify Labs, Inc
#include <stdio.h>
#include <unistd.h>

#include <chrono.hpp>
#include <sys.hpp>
#include <sos.hpp>
#include <var.hpp>

#include <lvgl_api.h>

#include "sl_config.h"

#include "Application.hpp"

static void print_usage();

class Options {
public:
  Options(const Cli &cli) { parse_string_argument(cli, "path", m_path); }

private:
  API_AC(Options, PathString, path);
  API_AC(Options, MicroTime, period);

  template <typename MemberType>
  void parse_string_argument(const Cli &cli, const char *option,
                             MemberType &member, const char *help = "") {
    if (const auto arg = cli.get_option(option, help);
        arg.is_empty() == false) {
      member = arg;
    }
  }
};

int main(int argc, char *argv[]) {

  Cli cli(argc, argv);
  const Options options(cli);

  if (cli.get_option("help") == "true") {
    cli.show_help(Cli::ShowHelp()
                      .set_publisher("Stratify Labs, Inc")
                      .set_version(SL_CONFIG_VERSION_STRING));
    exit(0);
  }

  lvgl::Runtime runtime;
  runtime.setup();
  Application().loop(runtime);
  return 0;
}
