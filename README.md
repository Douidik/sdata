# About sdata
The sdata language is a minimalist serialization language syntactically very close to JSON.  
The language is designed to be human-readable, performant, easy-to-use and quickly mastered.  
The source code is easily readable and the core doesn't rely on any libraries (fmt will be available soon on clang/gcc compiler as std::format).
## Syntax
```from 'examples/game.sd'```
```
tetris {
    window {
        width: 1920,
        height: 1080,
        title: 'Tetris game',
        fullscreen: false
    },
    controls {
        left: 'a',
        right: 'd',
        confirm: 'e',
        pause: 'p'
    }
}
```
### Data types
| Type             | Syntax                       | Example                            |
|------------------|------------------------------|------------------------------------|
| ```NIL```      | nil                          | the_void: nil                        |
| ```ARRAY```    | [\<element\>, \<next\>, ...] | even: [2,4,6,8,10,12]                |
| ```SEQUENCE``` | {\<member\>, \<next\>, ...}  | man { name: "John Doe", age: 32 }    |
| ```FLOAT```    | \<decimal\>f                 | { ratio: 0.8f, seconds: 5.4 }        |
| ```INT```      | \<integer\>                  | size: 2                              |
| ```BOOL```     | \<true/false\>               |  { is_open: true, is_closed: false } |
| ```STRING```   | \'\<data\>\' or \"\<data\>\" | { city: "Shanghai", native: \'上海\' }|

## Getting started
```from 'examples/game.sd'```
```cpp
#include <sdata/sdata.hpp>

// POD struct
struct Window {
  int width, height;
  std::string title {};
  bool fullscreen;
};

// Specializing sdata::Serializer for type <Window>
template<>
struct sdata::Serializer<Window> : std::true_type {
  // NOTE: Serializer<Window> definition is using the sdata's namespace implicitly

  // Deserialize a sdata::Node using the Window
  void encode(Node &node, const Window &window) {
    // Create the node hierarchy with a Sequence (collection of nodes)
    node = {
      "window",
      Sequence {
        {"width", window.width},
        {"height", window.height},
        {"title", window.title},
        {"fullscreen", window.fullscreen},
      },
    };
  }

  // Serialize a Window using a sdata::Node
  void decode(const Node &node, Window &window) {
    window = {
      // We access the targetted node with .at()
      // Then we grab the underlying value with get<T>()
      // Optionally you can get pass a default value to get
      .width = node.at("width").get<int>(800),
      .height = node.at("height").get<int>(600),
      .title = node.at("title").get<std::string>(),
      .fullscreen = node.at("fullscreen").get<bool>(),
    };
  }
};

int main() {
  // Read and parse the sdata source with parse_file
  // The conversion is implicit because we specialized Serializer<Window>
  Window window = sdata::parse_file("examples/game.sd").at("window");

  // Modify the window settings
  {
    window.width = 1280, window.height = 720;
    window.title = "Hello world";
    window.fullscreen = false;
  }

  // Deserialize the window to 'window.sd', the conversion must be explicit
  sdata::write_file("examples/window.sd", sdata::Node {window});
}

```
## sdata_format
sdata_format is a program to format your sdata source code.
```
# Usage
sdata_format -p <source filepath> -s <raw source> -f <format sdata source> -t <format template [classic/inlined/minimal]  
# Example
sdata_format -p examples/dialog.sd -t inlined
```
