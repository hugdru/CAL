#include "TxtMapParser.hpp"

#include <fstream>
#include <boost/algorithm/string.hpp>

using std::ifstream;

TxtMapParser::TxtMapParser(string *nodes_file_path_, string *roads_file_path_,
                           string *subroads_file_path_)
    : nodes_file_path{nodes_file_path_},
      roads_file_path{roads_file_path_},
      subroads_file_path{subroads_file_path_} {}

TxtMapParser::txt_parsed_t TxtMapParser::parse(void) {
  parse_nodes();
  parse_roads();
  parse_subroads();
  return this->parsed;
}

void TxtMapParser::parse_nodes(void) {

  ifstream node_file;
  node_file.exceptions(std::ifstream::badbit);

  node_file.open((*this->nodes_file_path).c_str());
  if (node_file.is_open()) {
    string node_str;
    while (std::getline(node_file, node_str)) {
      vector<std::string> node_arguments;
      boost::split(node_arguments, node_str, boost::is_any_of(";"));
      if (node_arguments.size() != 5) {
        throw std::invalid_argument("A node must have 5 arguments");
      }

      using std::stoll;
      using std::stod;
      long long int id = stoll(node_arguments[0]);
      double lat_deg = stod(node_arguments[1]);
      double lon_deg = stod(node_arguments[2]);
      double lat_rad = stod(node_arguments[3]);
      double lon_rad = stod(node_arguments[4]);
      this->parsed.node_vector.push_back(
          new Node{id, lat_deg, lon_deg, lat_rad, lon_rad});
    }
  }
}
void TxtMapParser::parse_roads(void) {

  // ifstream roads_file((*this->roads_file_path).c_str());
  // roads_file.exceptions(std::ofstream::failbit | std::ofstream::badbit);
  //
  // string node_str;
  // if (roads_file.is_open()) {
  //   while (std::getline(roads_file, node_str)) {
  //     vector<std::string> node_arguments;
  //     boost::split(node_arguments, node_str, boost::is_any_of(";"));
  //     if (node_arguments.size() != 5) {
  //       throw std::invalid_argument("A node must have 5 arguments");
  //     }
  //
  //     using std::stoll;
  //     using std::stod;
  //     long long int id = stoll(node_arguments[0]);
  //     double lat_deg = stod(node_arguments[1]);
  //     double lon_deg = stod(node_arguments[2]);
  //     double lat_rad = stod(node_arguments[3]);
  //     double lon_rad = stod(node_arguments[4]);
  //     this->parsed.node_vector.push_back(
  //         new Node{id, lat_deg, lon_deg, lat_rad, lon_rad});
  //   }
  // }
}
void TxtMapParser::parse_subroads(void) {}
