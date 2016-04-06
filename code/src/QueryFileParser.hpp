#ifndef QUERYFILEPARSER_HPP_
#define QUERYFILEPARSER_HPP_

#include <string>
#include <tuple>

#include <iostream>
#include <fstream>
#include <type_traits>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using std::string;
using std::tuple;

class QueryFileParser {
 public:
  enum class Field { URL = 0 };
  QueryFileParser(string query_file);

  template <QueryFileParser::Field F>
  auto& get();

  void parse();

 private:
  using tuple_t = tuple<string>;
  string query_file;
  tuple_t my_tuple;

  template <QueryFileParser::Field F, typename Value>
  void set(Value value);
};

using boost::property_tree::ptree;
using std::ifstream;

QueryFileParser::QueryFileParser(string query_file_) : query_file{query_file_} {
  if (query_file.empty()) {
    throw std::invalid_argument(
        "QueryFileParser expecting a valid: query_file.");
  }
}

void QueryFileParser::parse() {
  ifstream file{this->query_file.c_str()};
  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  if (file.is_open()) {
    ptree treePointer;
    read_json(file, treePointer);

    string holder =
        treePointer.get<string>("location") + treePointer.get<string>("query");

    this->set<QueryFileParser::Field::URL>(holder);

    file.close();
  }
}

template <QueryFileParser::Field F>
auto& QueryFileParser::get() {
  return std::get<static_cast<std::underlying_type_t<QueryFileParser::Field>>(
      F)>(this->my_tuple);
}

template <QueryFileParser::Field F, typename Value>
void QueryFileParser::set(Value value) {
  std::get<static_cast<std::underlying_type_t<QueryFileParser::Field>>(F)>(
      this->my_tuple) = value;
}

#endif /* QUERYFILEPARSER_HPP_ */
