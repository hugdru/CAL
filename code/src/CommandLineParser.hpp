#ifndef COMMANDLINEPARSER_HPP_
#define COMMANDLINEPARSER_HPP_

#include <string>
#include <unordered_map>
#include <initializer_list>

#include "WebFetch.hpp"

using std::string;
using std::unordered_map;

namespace std {
template <class E>
struct hash {
  inline typename std::enable_if_t<std::is_enum<E>::value, int> operator()(
      E const value) const {
    return static_cast<typename std::underlying_type_t<E>>(value);
  }
};
}

class CommandLineParser {
 private:
  int cardinality;
  char **arguments;
  string help =
      "( (--queryfile|-qf) queryfile [outputfile] ) | ( (--maptxtfiles|-mtf) nodesfile roadsfile waysfile "
      ")";

  void error(string error);
  void error();

 public:
  enum class Options {
    QUERY_FILE_PATH,
    QUERY_FILE_OUTPUT_PATH,
    MAP_NODE_FILE_PATH,
    MAP_ROAD_FILE_PATH,
    MAP_WAYS_FILE_PATH
  };
  CommandLineParser(const int cardinality, char **arguments);

  void parse(unordered_map<Options, string> &options);
};

#endif /* COMMANDLINEPARSER_HPP_ */
