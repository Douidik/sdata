#include "parser_test.hpp"
#include "regex_test.hpp"
#include "scanner_test.hpp"
#include "writer_test.hpp"
#include <catch2/catch_all.hpp>


int main(int argc, char **argv) {
  return Catch::Session().run(argc, argv);
}
