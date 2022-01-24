#include <sdata/sdata.hpp>

// Window properties pod struct
struct Window {
  unsigned width, height;
  std::string_view title;
  bool fullscreen;
};

// User-defined serialization derived from Scheme<...> for Window
// You can also derive the Serializer from Convert<...>
template<>
struct sdata::Serializer<Window> : Scheme<Window(unsigned, unsigned, std::string_view, bool)> {
  // We map the node to window properties.
  Map map(Window &window) override {
    return {
      {"width", window.width},
      {"height", window.height},
      {"title", window.title},
      {"fullscreen", window.fullscreen},
    };
  }
};

int main() {
  // Read and parse the sdata source with sdata::parse_file
  Window window = sdata::parse_file("examples/game.sd").at("window");

  // Tweak window settings
  window = Window {.width = 1280, .height = 720, .title = "Hello world"};

  // Deserialize the window instance to 'window.sd'
  sdata::write_file("examples/window.sd", sdata::Node {"window", window});
}