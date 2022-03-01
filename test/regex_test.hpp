#ifndef SDATA_REGEX_TEST_HPP
#define SDATA_REGEX_TEST_HPP

#include <catch2/catch.hpp>
#include <iomanip>
#include <iostream>
#include <sdata/regex/regex.hpp>

using namespace sdata;
using namespace sdata::regex_literals;

constexpr std::string_view LOREM_IPSUM = R"(
Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
labore et dolore magna aliqua. Id porta nibh venenatis cras sed felis eget velit. Viverra tellus
in hac habitasse. Sed risus pretium quam vulputate dignissim suspendisse in est. In eu mi
bibendum neque egestas congue quisque egestas. Mi proin sed libero enim sed faucibus turpis in.
Aliquam vestibulum morbi blandit cursus. Tellus in hac habitasse platea dictumst vestibulum.
Massa ultricies mi quis hendrerit. Molestie a iaculis at erat pellentesque adipiscing commodo.
Vulputate eu scelerisque felis imperdiet proin fermentum. Vitae congue eu consequat ac felis. Nec
ultrices dui sapien eget mi proin sed. Nunc mattis enim ut tellus elementum sagittis vitae et.
Mauris ultrices eros in cursus turpis massa tincidunt dui ut. Nisi porta lorem mollis aliquam ut
porttitor leo a diam. Diam phasellus vestibulum lorem sed risus ultricies. Arcu vitae elementum
curabitur vitae nunc sed velit dignissim. Ut eu sem integer vitae justo eget magna fermentum
iaculis.In eu mi bibendum neque.
)";

inline std::string quoted(std::string_view expression) {
  std::ostringstream ss;
  ss << std::quoted(expression, '\'');
  return ss.str();
}

TEST_CASE("Regex: Unknown tokens") {
  CHECK_THROWS_AS("N"_re, RegexParserException);
  CHECK_THROWS_AS(")"_re, RegexParserException);
  CHECK_THROWS_AS("ù"_re, RegexParserException);
}

TEST_CASE("Regex: Literals") {
  SECTION("Basic") {
    CHECK("'abc'"_re.match("abc"));
    CHECK("'abc'"_re.match("abcccccccccc"));
    CHECK("'hello' ' ' 'world'"_re.match("hello world"));
    CHECK("'hello\nworld'"_re.match("hello\nworld"));
    CHECK(Regex(quoted(LOREM_IPSUM)).match(LOREM_IPSUM));
  }

  SECTION("Invalid") {
    CHECK_THROWS_AS("'hello"_re, RegexParserException);
    CHECK_THROWS_AS("hello'"_re, RegexParserException);
    CHECK_THROWS_AS("hello"_re, RegexParserException);
  }

  SECTION("False") {
    CHECK_FALSE("'cba'"_re.match("abc"));
    CHECK_FALSE("'cbaa'"_re.match("abcc"));
    CHECK_FALSE(Regex(quoted(LOREM_IPSUM)).match(LOREM_IPSUM.substr(1)));
    CHECK_FALSE(Regex(quoted(LOREM_IPSUM)).match(LOREM_IPSUM.substr(0, LOREM_IPSUM.size() - 2)));
  }
}

TEST_CASE("Regex: Character classes") {
  SECTION("Basic") {
    CHECK("_"_re.match("\n"));
    CHECK("a"_re.match("a"));
    CHECK("o"_re.match("+"));
    CHECK("n"_re.match("7"));
    CHECK("Q"_re.match("\""));
    CHECK("q"_re.match("'"));
  }

  SECTION("False") {
    CHECK_FALSE("_"_re.match("b"));
    CHECK_FALSE("a"_re.match("4"));
    CHECK_FALSE("o"_re.match("\t"));
    CHECK_FALSE("n"_re.match("|"));
    CHECK_FALSE("Q"_re.match("^"));
    CHECK_FALSE("q"_re.match("&"));
  }
}

TEST_CASE("Regex: Sequences") {
  SECTION("Basic") {
    CHECK("{'abc'}"_re.match("abc"));
    CHECK("{'ab'} {'c'}"_re.match("abc"));
    CHECK("{{{{{{'ab'} {'c'}}}}}}"_re.match("abc"));
  }

  SECTION("Invalid") {
    CHECK_THROWS_AS("{'abc'"_re, RegexParserException);
    CHECK_THROWS_AS("{"_re, RegexParserException);
    CHECK_THROWS_AS("}"_re, RegexParserException);
    CHECK_THROWS_AS("{{{'abc'"_re, RegexParserException);
    CHECK_THROWS_AS("'abc'}}}"_re, RegexParserException);
  }
}

TEST_CASE("Regex: Quantifier plus") {
  SECTION("Basic") {
    CHECK("{'abc'}+"_re.match("abcabcabc"));
    CHECK("{'ab'n}+"_re.match("ab1ab2ab3"));
    CHECK("n+n+"_re.match("12"));
  }

  SECTION("Invalid") {
    CHECK_THROWS_AS("+"_re, RegexParserException);
    CHECK_THROWS_AS("++"_re, RegexParserException);
    CHECK_THROWS_AS("+a"_re, RegexParserException);
    CHECK_THROWS_AS("{}+"_re, RegexParserException);
  }
}

TEST_CASE("Regex: Quantifier kleene") {
  SECTION("Basic") {
    CHECK("{'abc'}*"_re.match("abc"));
    CHECK("{'abc'}*"_re.match(""));
    CHECK("{'ab'n}*"_re.match("ab1ab2ab3"));
    CHECK("{{{'hello'}}}*"_re.match(""));
    CHECK("{{{'hello'}}}*"_re.match("hellohellohello"));
  }

  SECTION("Invalid") {
    CHECK_THROWS_AS("*"_re, RegexParserException);
    CHECK_THROWS_AS("***"_re, RegexParserException);
    CHECK_THROWS_AS("*a"_re, RegexParserException);
    CHECK_THROWS_AS("{}*"_re, RegexParserException);
  }
}

TEST_CASE("Regex: Quantifier quest") {
  SECTION("Basic") {
    CHECK("{'abc'}?"_re.match("abc"));
    CHECK("{'abc'}?"_re.match(""));
    CHECK("{'ab'n}?"_re.match("ab1"));
    CHECK("{{{'hello'}}}?"_re.match(""));
    CHECK("{{{'hello'}}}?"_re.match("hello"));
  }

  SECTION("Invalid") {
    CHECK_THROWS_AS("?"_re, RegexParserException);
    CHECK_THROWS_AS("???"_re, RegexParserException);
    CHECK_THROWS_AS("?a"_re, RegexParserException);
    CHECK_THROWS_AS("{}?"_re, RegexParserException);
  }
}

TEST_CASE("Regex: Alternative") {
  SECTION("Basic") {
    CHECK("{'a'|'b'}"_re.match("a"));
    CHECK("{'a'|'b'}"_re.match("a"));
    CHECK("{'a' | 'b'}"_re.match("a"));
    CHECK("{'a' | 'b'}"_re.match("b"));
    CHECK("a{a|'_'|n}*"_re.match("snake_case_variable123"));
  }

  SECTION("Invalid") {
    CHECK_THROWS_AS("|"_re, RegexParserException);
    CHECK_THROWS_AS("||"_re, RegexParserException);
    CHECK_THROWS_AS("|||"_re, RegexParserException);

    CHECK_THROWS_AS("'a'|{}"_re, RegexParserException);
    CHECK_THROWS_AS("{}|'b'"_re, RegexParserException);
    CHECK_THROWS_AS("'a'|"_re, RegexParserException);
    CHECK_THROWS_AS("|'b'"_re, RegexParserException);
  }
}

TEST_CASE("Regex: Wave") {
  CHECK("{'a'~'f'}"_re.match("abcdef"));
  CHECK_THROWS_AS("{'a'~{}}"_re.match("abcdef"), RegexParserException);
}

#endif
