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
using std::make_unique;
using std::make_shared;

class TxtMapParser {
 public:
  using txt_parsed_t = struct {
    unordered_map<long long int, Node *> nodes_umap;
    unordered_map<long long int, Road *> roads_umap;
    vector<Subroad *> subroads_vector;
  };

 private:
  string const *const nodes_file_path;
  string const *const roads_file_path;
  string const *const subroads_file_path;
  txt_parsed_t parsed;

 public:
  TxtMapParser(string const *const nodes_file_path_,
               string const *const roads_file_path_,
               string const *const subroads_file_path_);
  ~TxtMapParser();

  void parse_nodes(void);
  void parse_roads(void);
  void parse_subroads(void);

  txt_parsed_t parse(void);
};
#endif /* TXTMAPPARSER_HPP_ */
