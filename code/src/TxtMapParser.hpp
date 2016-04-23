#ifndef TXTMAPPARSER_HPP_
#define TXTMAPPARSER_HPP_

#include <string>
#include <vector>
#include <memory>

#include "Node.hpp"

using std::string;
using std::vector;
using std::unique_ptr;

class TxtMapParser {
 public:
  using txt_parsed_t = struct { vector<Node*> node_vector; };

 private:
  string* nodes_file_path;
  string* roads_file_path;
  string* subroads_file_path;
  txt_parsed_t parsed;

 public:
  TxtMapParser(string* nodes_file_path_, string* roads_file_path_,
               string* subroads_file_path_);

  void parse_nodes(void);
  void parse_roads(void);
  void parse_subroads(void);

  txt_parsed_t parse(void);
};
#endif /* TXTMAPPARSER_HPP_ */
