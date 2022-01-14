#include "regex_automata.hpp"
#include <map>

namespace sdata {

RegexNode *RegexAutomata::insert(
  const char *state,
  const std::set<RegexNode *, RegexNode::Compare> &&ancestors,
  const std::set<RegexNode *, RegexNode::Compare> &&edges) {
  auto [node, _] = m_nodes.insert(new RegexNode {
    state,
    edges,
    size(),
  });

  for (RegexNode *ancestor : ancestors) {
    ancestor->edges.insert(*node);
  }

  return *node;
}

RegexNode *RegexAutomata::merge(
  const RegexAutomata &automata,
  std::set<RegexNode *, RegexNode::Compare> &&ancestors) {
  RegexNode *merged = insert_automata(automata);

  if (merged != nullptr) {
    for (RegexNode *ancestor : ancestors) {
      ancestor->edges.insert(merged);
    }
  }

  return merged;
}

RegexNode *RegexAutomata::insert_automata(const RegexAutomata &automata) {
  // Map the original node to the merged one in order to rebuild the hierarchy
  std::map<RegexNode *, RegexNode *, RegexNode::Compare> map {};

  for (RegexNode *node : automata.nodes()) {
    map.insert({
      node,
      insert(node->state, {}, {}),
    });
  }

  for (auto [node, merged] : map) {
    for (RegexNode *edge : node->edges) {
      merged->edges.insert(map.at(edge));
    }
  }

  return !map.empty() ? map.begin()->second : nullptr;
}

}  // namespace sdata
