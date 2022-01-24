#include <cxxopts.hpp>
#include <sdata/sdata.hpp>

namespace sdata_format {

namespace fs = std::filesystem;
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
        "s,source",
        "sdata source filepath | -s <filepath>",
        cxxopts::value<fs::path>(),
      },
      {
        "r,raw",
        R"(raw sdata source within quotes | -r "<raw source>")",
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

  if (arguments.count("source")) {
    request.source = sdata::read_file(arguments["source"].as<fs::path>());
  }

  if (arguments.count("raw")) {
    request.source = arguments["raw"].as<std::string>();
  }

  if (arguments.count("format")) {
    fs::path path = arguments["format"].as<fs::path>();
    request.format = sdata::parse_file(path);
  }

  if (arguments.count("template") && !arguments.count("format")) {
    static const std::unordered_map<std::string_view, sdata::Format> TEMPLATE {
      {"classic", sdata::Format::classic()},
      {"inlined", sdata::Format::inlined()},
      {"minimal", sdata::Format::minimal()},
    };

    std::string_view name = arguments["template"].as<std::string>();

    if (auto iter = TEMPLATE.find(name); iter != TEMPLATE.end()) {
      request.format = iter->second;
    } else {
      throw Exception {fmt("Unrecognized format template '{}'", name)};
    }
  }

  if (request.source.empty()) {
    throw Exception {"No source specified, please provide one with -s or -r"};
  }

  return request;
}

std::string format(const Request &request) {
  const auto node = sdata::parse_str(request.source);
  return write_str(node, request.format);
}

}  // namespace sdata_format

int main(int argc, char **argv) {
  try {
    auto arguments = sdata_format::create_options().parse(argc, argv);
    auto request = sdata_format::parse_arguments(arguments);
    std::cout << sdata_format::format(request) << std::endl;
  } catch (const std::exception &exception) {
    std::cerr << exception.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
