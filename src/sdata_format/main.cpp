#include <cxxopts.hpp>
#include <sdata/sdata.hpp>

namespace fs = std::filesystem;

namespace sdata_format {

using sdata::fmt;

class Exception : public sdata::Exception {
public:
  Exception(std::string_view description) : sdata::Exception(message(description)) {}

private:
  static std::string message(std::string_view description) {
    return fmt("[sdata_format::Exception raised] {}", description);
  }
};

struct Request {
  std::string source {};
  sdata::Format format;
};

cxxopts::Options create_options() {
  cxxopts::Options options {"sdata format", "format your sdata source code"};

  options.add_options(
    "formatting",
    {
      {
        "p,path",
        "sdata source filepath | -p <path>",
        cxxopts::value<fs::path>(),
      },
      {
        "s,source",
        R"(raw sdata source within quotes | -s "<source>")",
        cxxopts::value<std::string>(),
      },
      {
        "f,format",
        "sdata format source filepath | -f <path>",
        cxxopts::value<fs::path>(),
      },
      {
        "t,template",
        "format template [classic/inlined/minimal] | -t <name>",
        cxxopts::value<std::string>()->default_value("classic"),
      },
    });

  return options;
}

Request parse_arguments(cxxopts::ParseResult &arguments) {
  Request request {};

  if (arguments.count("path")) {
    request.source = sdata::read_file(arguments["path"].as<fs::path>());
  }

  if (arguments.count("source")) {
    request.source = arguments["source"].as<std::string>();
  }

  if (arguments.count("format")) {
    auto node = sdata::parse_file(arguments["format"].as<fs::path>());
    sdata::Serializer<sdata::Format>().encode(node, request.format);
  }

  {
    static const std::unordered_map<std::string_view, sdata::Format> TEMPLATE {
      {"classic", sdata::Format::classic()},
      {"inlined", sdata::Format::inlined()},
      {"minimal", sdata::Format::minimal()},
    };

    std::string_view name = arguments["template"].as<std::string>();

    if (auto iter = TEMPLATE.find(name); iter != TEMPLATE.end()) {
      request.format = iter->second;
    } else {
      throw Exception {fmt("No format template named '{}'", name)};
    }
  }

  return request;
}

std::string perform_request(const Request &request) {
  const auto node = sdata::parse_str(request.source);
  return write_str(node, request.format);
}

}  // namespace sdata_format

int main(int argc, char **argv) {
  try {
    auto arguments = sdata_format::create_options().parse(argc, argv);
    auto request = sdata_format::parse_arguments(arguments);
    std::cout << sdata_format::perform_request(request) << std::endl;
  } catch (const std::exception &exception) {
    std::cerr << exception.what() << std::endl;
  }

  return EXIT_SUCCESS;
}

// using sdata::fmt;

// class Exception : public sdata::Exception {
// public:
//   Exception(const std::string_view description) : sdata::Exception(message(description)) {}

// private:
//   inline static std::string message(std::string_view description) {
//     return sdata::fmt("[sdata_format::Exception raised] {} \n", description);
//   }
// };

// struct Option {
//   enum Type { FORMAT = 1, TEMPLATE = 2, SRC = 4 } type;
//   std::string_view name, shortcut;
// };

// struct Argument {
//   Option option;
//   std::string_view data;
// };

// struct Request {
//   std::filesystem::path path;
//   sdata::Format format;
// };

// Argument match_argument(std::string_view str) {
//   constexpr std::array<Option, 3> OPTIONS = {
//     Option {Option::FORMAT, "--format", "-f"},
//     Option {Option::TEMPLATE, "--template", "-t"},
//     Option {Option::SRC, "--src", "-s"},
//   };

//   Argument argument {};

//   for (const auto &option : OPTIONS) {
//     if (str.starts_with(option.name) || str.starts_with(option.name)) {
//       argument.option = option;
//       size_t set_position = str.find('=');

//       if (set_position == str.npos) {
//         throw Exception {fmt("Cant find '=' token in argument: '{}'", str)};
//       }

//       argument.data = str.substr(set_position, str.size());
//       return argument;
//     }
//   }

//   throw Exception {fmt("Unrecognized argument '{}'", str)};
// }

// Request parse_args(int argc, char **argv) {
//   Request request {"", sdata::Format::classic()};
//   unsigned matched_types = 0;

//   for (size_t i = 1; i < argc; i++) {
//     auto argument = match_argument(argv[i]);

//     if (argument.option.type & matched_types) {
//       const auto &opt = argument.option;
//       throw Exception {fmt("Argument '{}' or '{}' already specified", opt.name, opt.shortcut)};
//     }

//     switch (argument.option.type) {
//       case Option::FORMAT: {
//         const auto node = sdata::parse_file(argument.data);
//         sdata::Serializer<sdata::Format>().decode(node, request.format);
//       } break;

//       case Option::TEMPLATE: {
//         if (argument.data == "classic") {
//           request.format = sdata::Format::classic();
//         } else if (argument.data == "inlined") {
//           request.format = sdata::Format::inlined();
//         } else if (argument.data == "minimal") {
//           request.format = sdata::Format::minimal();
//         } else {
//           throw Exception {fmt("Unknown format template named '{}'", argument.data)};
//         }
//       } break;

//       case Option::SRC: {
//         request.path = argument.data;
//       } break;
//     }

//     matched_types |= argument.option.type;

//     if (matched_types & Option::FORMAT && matched_types & Option::TEMPLATE) {
//       throw Exception {"Arguments '--format' and '--template' are muttualy exclusive"};
//     }
//   }

//   if (!(matched_types & Option::SRC)) {
//     throw Exception {"Please provide the target with '-s=<path>' or '--src=<path>'"};
//   }

//   return request;
// }

// }  // namespace sdata_format

// int main(int argc, char **argv) {
//   try {
//     using namespace sdata_format;

//     Request request = parse_args(argc, argv);
//     auto node = sdata::parse_file(request.path);
//     sdata::write_file(request.path, node, request.format);
//   } catch (const std::exception &exception) {
//     std::cerr << exception.what() << std::endl;
//     return EXIT_FAILURE;
//   }

//   return EXIT_SUCCESS;
// }
