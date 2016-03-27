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
  inline typename std::enable_if_t<std::is_enum<E>::value, std::size_t>
  operator()(E const value) const {
    return static_cast<typename std::underlying_type_t<E>>(value);
  }
};
}

class CommandLineParser {
 private:
  int cardinality;
  char **arguments;
  string help = "[(--queryfile|-qf) [--mapfile|-mf]] [(--mapfile|-mf)]";

  void error(string error);
  void error();

 public:
  enum class Options {
    QUERY_FILE_PATH,
    MAP_FILE_PATH,
  };
  CommandLineParser(const int cardinality, char **arguments);

  void parse(unordered_map<Options, string> &options);
};

#endif /* COMMANDLINEPARSER_HPP_ */
