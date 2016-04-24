#ifndef TXTMAPPARSER_HPP_
#define TXTMAPPARSER_HPP_

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include "Node.hpp"
#include "Road.hpp"
#include "Subroad.hpp"

using std::string;
using std::vector;
using std::unordered_map;
using std::unique_ptr;
using std::shared_ptr;

class TxtMapParser {
 public:
  using txt_parsed_t = struct {
    unordered_map<long long int, shared_ptr<Node>> nodes_umap;
    unordered_map<long long int, shared_ptr<Road>> roads_umap;
    vector<shared_ptr<Subroad>> subroads_vector;
  };

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
