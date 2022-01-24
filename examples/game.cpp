#include <sdata/sdata.hpp>

// Window properties struct
struct Window {
  unsigned width, height;
  std::string_view title {};
  bool fullscreen;
};

template<>
struct sdata::Serializer<Window> : Scheme<Window(unsigned, unsigned, std::string_view, bool)> {
  Map map(Window &window) override {
    return {
      {"width", window.width},
      {"height", window.height},
      {"title", window.title},
      {"fullscreen", window.fullscreen},
    };
  }
};

void from_file() {
  // Read and parse the sdata source with sdata::parse_file
  Window window = sdata::parse_file("examples/game.sd").at("window");

  // Tweak window settings
  window = Window {.width = 1280, .height = 720, .title = "Hello world"};

  // Deserialize the window to 'window.sd'
  sdata::write_file("examples/window.sd", sdata::Node {"window", window});
}

std::string from_node() {
  // Construct the node hierarchy
  Node window {
    "window",
    {
      {"width", 1280},
      {"height", 720},
      {"fullscreen", true},
    },
  };

  // Use the insertion operator to append a new node to the sequence
  window["title"] = "Hello world";
  // Access the "fullscreen" node with .at() and change its value
  window.at("fullscreen") = false;

  // Return the window source code
  return sdata::write_str(window);
}
