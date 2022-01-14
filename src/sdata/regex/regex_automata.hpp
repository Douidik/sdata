#ifndef SDATA_REGEX_AUTOMATA_HPP
#define SDATA_REGEX_AUTOMATA_HPP

#include "misc/assert.hpp"
#include "regex_match.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>
#include <string_view>

namespace sdata {

// Non-literal regex states:
static const char *REGEX_EPSILON = (const char *)(0x0);
static const char *REGEX_ANY = (const char *)(0x1);

struct RegexNode {
  struct Compare {
    inline bool operator()(const RegexNode *a, const RegexNode *b) const {
      return a->id < b->id;
    }
  };

  template<typename T>
  inline bool accepts(const T input, const T end) const {
    return state == REGEX_EPSILON || (input != end) && (state == REGEX_ANY || *state == *input);
  }

  inline bool is_leaf() const {
    return edges.empty() || (*edges.rbegin())->id <= id;
  }

  std::set<RegexNode *, Compare> leaves() {
    if (is_leaf()) {
      return {this};
    }

    std::set<RegexNode *, Compare> edge_leaves {};

    for (RegexNode *edge : edges) {
      if (edge->id > id)
        edge_leaves.merge(edge->leaves());
    }

    return edge_leaves;
  }

  const char *state;
  std::set<RegexNode *, Compare> edges;
  size_t id;
};

class RegexAutomata {
public:
  RegexAutomata() = default;
  RegexAutomata &operator=(const RegexAutomata &) = delete;

  RegexAutomata(const RegexAutomata &automata) {
    insert_automata(automata);
  }

  ~RegexAutomata() {
    for (RegexNode *node : m_nodes)
      delete node;
  }

  RegexNode *insert(
    const char *state,
    const std::set<RegexNode *, RegexNode::Compare> &&ancestors,
    const std::set<RegexNode *, RegexNode::Compare> &&edges);

  RegexNode *
  merge(const RegexAutomata &automata, std::set<RegexNode *, RegexNode::Compare> &&ancestors);

  template<typename T>
  RegexMatch run(T begin, T input, const T end, const RegexNode *node) const {
    if (node != nullptr && node->accepts(input, end)) {
      T output = (node->state != REGEX_EPSILON) ? input + 1 : input;

      for (RegexNode *edge : node->edges) {
        if (RegexMatch match = run(begin, output, end, edge)) {
          return match;
        }
      }

      if (node->state != REGEX_ANY && node->is_leaf()) {
        return {true, (size_t)std::distance(begin, output)};
      }
    }

    return {false, (size_t)std::distance(begin, input)};
  }

  inline RegexNode *root() const {
    return *m_nodes.begin();
  }

  inline const auto nodes() const {
    return m_nodes;
  }

  inline size_t size() const {
    return m_nodes.size();
  }

  inline bool empty() const {
    return m_nodes.empty();
  }

  inline std::set<RegexNode *, RegexNode::Compare> leaves() {
    return root() != nullptr ? root()->leaves() : std::set<RegexNode *, RegexNode::Compare> {};
  }

private:
  RegexNode *insert_automata(const RegexAutomata &automata);
  std::set<RegexNode *, RegexNode::Compare> m_nodes;
};

}  // namespace sdata

#endif
