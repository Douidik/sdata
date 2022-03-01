#ifndef SDATA_SCANNER_TEST_HPP
#define SDATA_SCANNER_TEST_HPP

#include <catch2/catch.hpp>
#include <sdata/sdata.hpp>

using namespace sdata;

TEST_CASE("Scanner<char> patterns") {
  SECTION("empty") {
    const auto &empty_pattern = Token::PATTERN.at(Token::EMPTY);
    CHECK(empty_pattern.match(" "));
    CHECK(empty_pattern.match("   "));
    CHECK(empty_pattern.match("\n "));
    CHECK(empty_pattern.match("\t\t\n   "));
    CHECK_FALSE(empty_pattern.match("@namespace"));
  }

  SECTION("string") {
    const auto &string_pattern = Token::PATTERN.at(Token::STRING);
    CHECK(string_pattern.match("'hello world'"));
    CHECK(string_pattern.match("'hello\nworld'"));
    CHECK(string_pattern.match("'c'"));
    CHECK_FALSE(string_pattern.match("hello"));
  }

  SECTION("int") {
    const auto &integer_pattern = Token::PATTERN.at(Token::INT);
    CHECK(integer_pattern.match("1"));
    CHECK(integer_pattern.match("134342"));
    CHECK(integer_pattern.match("00313134"));
  }

  SECTION("float") {
    const auto &float_pattern = Token::PATTERN.at(Token::FLOAT);
    CHECK(float_pattern.match("1.0f"));
    CHECK(float_pattern.match("13243.43934f"));
    CHECK(float_pattern.match("0.0003923423493423f"));
    CHECK(float_pattern.match("0.0"));
    CHECK_FALSE(float_pattern.match("95435"));
    CHECK_FALSE(float_pattern.match(".0f"));
    CHECK_FALSE(float_pattern.match("0.f"));
  }

  SECTION("id") {
    const auto &id_pattern = Token::PATTERN.at(Token::ID);
    CHECK(id_pattern.match("my_identifier"));
    CHECK(id_pattern.match("my_id_nb123"));
    CHECK_FALSE(id_pattern.match("12identifier"));
  }

  SECTION("operators") {
    CHECK(Token::PATTERN.at(Token::SET).match(":"));
    CHECK(Token::PATTERN.at(Token::BEG_SEQ).match("{"));
    CHECK(Token::PATTERN.at(Token::END_SEQ).match("}"));
    CHECK(Token::PATTERN.at(Token::BEG_ARR).match("["));
    CHECK(Token::PATTERN.at(Token::END_ARR).match("]"));
    CHECK(Token::PATTERN.at(Token::SEPARATOR).match(","));
  }

  SECTION("keywords") {
    CHECK(Token::PATTERN.at(Token::TRUE).match("true"));
    CHECK(Token::PATTERN.at(Token::FALSE).match("false"));
    CHECK(Token::PATTERN.at(Token::NIL).match("nil"));
  }
}

static bool
token_matches(Scanner &scanner, std::pair<std::string_view, Token::Category> &&expected) {
  auto token = scanner.tokenize();
  return expected.first == token.expression && expected.second == token.category;
}

TEST_CASE("Scanner for 'game.sd'") {
  auto source = read_file("examples/game.sd");
  Scanner scanner {source};

  REQUIRE(token_matches(scanner, {"tetris", Token::ID}));
  REQUIRE(token_matches(scanner, {"{", Token::BEG_SEQ}));
  {
    REQUIRE(token_matches(scanner, {"window", Token::ID}));
    REQUIRE(token_matches(scanner, {"{", Token::BEG_SEQ}));
    {
      REQUIRE(token_matches(scanner, {"width", Token::ID}));
      REQUIRE(token_matches(scanner, {":", Token::SET}));
      REQUIRE(token_matches(scanner, {"1920", Token::INT}));
      REQUIRE(token_matches(scanner, {",", Token::SEPARATOR}));

      REQUIRE(token_matches(scanner, {"height", Token::ID}));
      REQUIRE(token_matches(scanner, {":", Token::SET}));
      REQUIRE(token_matches(scanner, {"1080", Token::INT}));
      REQUIRE(token_matches(scanner, {",", Token::SEPARATOR}));

      REQUIRE(token_matches(scanner, {"title", Token::ID}));
      REQUIRE(token_matches(scanner, {":", Token::SET}));
      REQUIRE(token_matches(scanner, {"'Tetris game'", Token::STRING}));
      REQUIRE(token_matches(scanner, {",", Token::SEPARATOR}));

      REQUIRE(token_matches(scanner, {"fullscreen", Token::ID}));
      REQUIRE(token_matches(scanner, {":", Token::SET}));
      REQUIRE(token_matches(scanner, {"false", Token::FALSE}));

      REQUIRE(token_matches(scanner, {"}", Token::END_SEQ}));
      REQUIRE(token_matches(scanner, {",", Token::SEPARATOR}));
    }

    REQUIRE(token_matches(scanner, {"controls", Token::ID}));
    REQUIRE(token_matches(scanner, {"{", Token::BEG_SEQ}));
    {
      REQUIRE(token_matches(scanner, {"left", Token::ID}));
      REQUIRE(token_matches(scanner, {":", Token::SET}));
      REQUIRE(token_matches(scanner, {"'a'", Token::STRING}));
      REQUIRE(token_matches(scanner, {",", Token::SEPARATOR}));

      REQUIRE(token_matches(scanner, {"right", Token::ID}));
      REQUIRE(token_matches(scanner, {":", Token::SET}));
      REQUIRE(token_matches(scanner, {"'d'", Token::STRING}));
      REQUIRE(token_matches(scanner, {",", Token::SEPARATOR}));

      REQUIRE(token_matches(scanner, {"confirm", Token::ID}));
      REQUIRE(token_matches(scanner, {":", Token::SET}));
      REQUIRE(token_matches(scanner, {"'e'", Token::STRING}));
      REQUIRE(token_matches(scanner, {",", Token::SEPARATOR}));

      REQUIRE(token_matches(scanner, {"pause", Token::ID}));
      REQUIRE(token_matches(scanner, {":", Token::SET}));
      REQUIRE(token_matches(scanner, {"'p'", Token::STRING}));

      REQUIRE(token_matches(scanner, {"}", Token::END_SEQ}));
    }

    REQUIRE(token_matches(scanner, {"}", Token::END_SEQ}));
  }
}

TEST_CASE("Scanner for 'dialog.sd'") {
  auto source = read_file("examples/dialog.sd");
  Scanner scanner {source};

  REQUIRE(token_matches(scanner, {"{", Token::BEG_SEQ}));

  REQUIRE(token_matches(scanner, {"en_US", Token::ID}));
  REQUIRE(token_matches(scanner, {"{", Token::BEG_SEQ}));
  {
    REQUIRE(token_matches(scanner, {"title", Token::ID}));
    REQUIRE(token_matches(scanner, {":", Token::SET}));
    REQUIRE(token_matches(scanner, {"'Game over'", Token::STRING}));
    REQUIRE(token_matches(scanner, {",", Token::SEPARATOR}));

    REQUIRE(token_matches(scanner, {"play_again_prompt", Token::ID}));
    REQUIRE(token_matches(scanner, {":", Token::SET}));
    REQUIRE(token_matches(scanner, {"'Play again ?'", Token::STRING}));
    REQUIRE(token_matches(scanner, {",", Token::SEPARATOR}));

    REQUIRE(token_matches(scanner, {"play_again_accept", Token::ID}));
    REQUIRE(token_matches(scanner, {":", Token::SET}));
    REQUIRE(token_matches(scanner, {"'Yes'", Token::STRING}));
    REQUIRE(token_matches(scanner, {",", Token::SEPARATOR}));

    REQUIRE(token_matches(scanner, {"play_again_refuse", Token::ID}));
    REQUIRE(token_matches(scanner, {":", Token::SET}));
    REQUIRE(token_matches(scanner, {"'No'", Token::STRING}));

    REQUIRE(token_matches(scanner, {"}", Token::END_SEQ}));
    REQUIRE(token_matches(scanner, {",", Token::SEPARATOR}));
  }

  REQUIRE(token_matches(scanner, {"fr_FR", Token::ID}));
  REQUIRE(token_matches(scanner, {"{", Token::BEG_SEQ}));
  {
    REQUIRE(token_matches(scanner, {"title", Token::ID}));
    REQUIRE(token_matches(scanner, {":", Token::SET}));
    REQUIRE(token_matches(scanner, {"'Partie terminée'", Token::STRING}));
    REQUIRE(token_matches(scanner, {",", Token::SEPARATOR}));

    REQUIRE(token_matches(scanner, {"play_again_prompt", Token::ID}));
    REQUIRE(token_matches(scanner, {":", Token::SET}));
    REQUIRE(token_matches(scanner, {"'Souhaitez-vous rejouer ?'", Token::STRING}));
    REQUIRE(token_matches(scanner, {",", Token::SEPARATOR}));

    REQUIRE(token_matches(scanner, {"play_again_accept", Token::ID}));
    REQUIRE(token_matches(scanner, {":", Token::SET}));
    REQUIRE(token_matches(scanner, {"'Oui'", Token::STRING}));
    REQUIRE(token_matches(scanner, {",", Token::SEPARATOR}));

    REQUIRE(token_matches(scanner, {"play_again_refuse", Token::ID}));
    REQUIRE(token_matches(scanner, {":", Token::SET}));
    REQUIRE(token_matches(scanner, {"'Non'", Token::STRING}));
    REQUIRE(token_matches(scanner, {"}", Token::END_SEQ}));
    REQUIRE(token_matches(scanner, {",", Token::SEPARATOR}));
  }

  REQUIRE(token_matches(scanner, {"es_ES", Token::ID}));
  REQUIRE(token_matches(scanner, {"{", Token::BEG_SEQ}));
  {
    REQUIRE(token_matches(scanner, {"title", Token::ID}));
    REQUIRE(token_matches(scanner, {":", Token::SET}));
    REQUIRE(token_matches(scanner, {"'Juego terminado'", Token::STRING}));
    REQUIRE(token_matches(scanner, {",", Token::SEPARATOR}));

    REQUIRE(token_matches(scanner, {"play_again_prompt", Token::ID}));
    REQUIRE(token_matches(scanner, {":", Token::SET}));
    REQUIRE(token_matches(scanner, {"'Juega de nuevo ?'", Token::STRING}));
    REQUIRE(token_matches(scanner, {",", Token::SEPARATOR}));

    REQUIRE(token_matches(scanner, {"play_again_accept", Token::ID}));
    REQUIRE(token_matches(scanner, {":", Token::SET}));
    REQUIRE(token_matches(scanner, {"'Sí'", Token::STRING}));
    REQUIRE(token_matches(scanner, {",", Token::SEPARATOR}));

    REQUIRE(token_matches(scanner, {"play_again_refuse", Token::ID}));
    REQUIRE(token_matches(scanner, {":", Token::SET}));
    REQUIRE(token_matches(scanner, {"'No'", Token::STRING}));

    REQUIRE(token_matches(scanner, {"}", Token::END_SEQ}));
    REQUIRE(token_matches(scanner, {",", Token::SEPARATOR}));
  }

  REQUIRE(token_matches(scanner, {"zh_CN", Token::ID}));
  REQUIRE(token_matches(scanner, {"{", Token::BEG_SEQ}));
  {
    REQUIRE(token_matches(scanner, {"title", Token::ID}));
    REQUIRE(token_matches(scanner, {":", Token::SET}));
    REQUIRE(token_matches(scanner, {"'游戏结束'", Token::STRING}));
    REQUIRE(token_matches(scanner, {",", Token::SEPARATOR}));

    REQUIRE(token_matches(scanner, {"play_again_prompt", Token::ID}));
    REQUIRE(token_matches(scanner, {":", Token::SET}));
    REQUIRE(token_matches(scanner, {"'再玩一次 ？'", Token::STRING}));
    REQUIRE(token_matches(scanner, {",", Token::SEPARATOR}));

    REQUIRE(token_matches(scanner, {"play_again_accept", Token::ID}));
    REQUIRE(token_matches(scanner, {":", Token::SET}));
    REQUIRE(token_matches(scanner, {"'是的'", Token::STRING}));
    REQUIRE(token_matches(scanner, {",", Token::SEPARATOR}));

    REQUIRE(token_matches(scanner, {"play_again_refuse", Token::ID}));
    REQUIRE(token_matches(scanner, {":", Token::SET}));
    REQUIRE(token_matches(scanner, {"'不'", Token::STRING}));

    REQUIRE(token_matches(scanner, {"}", Token::END_SEQ}));
  }

  REQUIRE(token_matches(scanner, {"}", Token::END_SEQ}));
}

#endif
