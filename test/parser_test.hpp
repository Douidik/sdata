#ifndef SDATA_PARSER_TEST_HPP
#define SDATA_PARSER_TEST_HPP

#include <catch2/catch_all.hpp>
#include <sdata/sdata.hpp>
#include <sstream>

using namespace sdata;

static const Node game {
  "tetris",
  Sequence {
    {
      "window",
      Sequence {
        {"width", 1920},
        {"height", 1080},
        {"title", "Tetris game"},
        {"fullscreen", false},
      },
    },
    {
      "controls",
      Sequence {
        {"left", "a"},
        {"right", "d"},
        {"confirm", "e"},
        {"pause", "p"},
      },
    },
  },
};

static const Node dialog {
  "",
  Sequence {
    {
      "en_US",
      Sequence {
        {"title", "Game over"},
        {"play_again_prompt", "Play again ?"},
        {"play_again_accept", "Yes"},
        {"play_again_refuse", "No"},
      },
    },
    {
      "fr_FR",
      Sequence {
        {"title", "Partie terminée"},
        {"play_again_prompt", "Souhaitez-vous rejouer ?"},
        {"play_again_accept", "Oui"},
        {"play_again_refuse", "Non"},
      },
    },
    {
      "es_ES",
      Sequence {
        {"title", "Juego terminado"},
        {"play_again_prompt", "Juega de nuevo ?"},
        {"play_again_accept", "Sí"},
        {"play_again_refuse", "No"},
      },
    },
    {
      "zh_CN",
      Sequence {
        {"title", "游戏结束"},
        {"play_again_prompt", "再玩一次 ？"},
        {"play_again_accept", "是的"},
        {"play_again_refuse", "不"},
      },
    },
  },
};

static const Node features = {
  "",
  Sequence {
    {
      "colors",
      Sequence {
        {"gray", Array {{128}, {128}, {128}}},
        {"blue", "#0000FF"},
        {"yellow", 16776960},
      },
    },
    {
      "vietnamese",
      R"(Lorem Ipsum chỉ đơn giản là một đoạn văn bản giả, được dùng vào việc trình bày và dàn trang phục vụ cho in ấn.
 Lorem Ipsum đã được sử dụng như một văn bản chuẩn cho ngành công nghiệp in ấn từ những năm 1500, khi một họa sĩ vô
 danh ghép nhiều đoạn văn bản với nhau để tạo thành một bản mẫu văn bản. Đoạn văn bản này không những đã tồn tại năm thế
 kỉ, mà khi được áp dụng vào tin học văn phòng, nội dung của nó vẫn không hề bị thay đổi. Nó đã được phổ biến trong
 những năm 1960 nhờ việc bán những bản giấy Letraset in những đoạn Lorem Ipsum, và gần đây hơn, được sử dụng trong các
 ứng dụng dàn trang.)",
    },
  },
};

TEST_CASE("Parser") {
  CHECK(parse_file("examples/game.sd").compare(game));
  CHECK(parse_file("examples/dialog.sd").compare(dialog));
  CHECK(parse_file("examples/features.sd").compare(features));
}

#endif
