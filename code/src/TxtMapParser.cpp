#include "TxtMapParser.hpp"

#include <fstream>
#include <boost/algorithm/string.hpp>

using std::ifstream;

TxtMapParser::TxtMapParser(string *nodes_file_path_, string *roads_file_path_,
                           string *ways_file_path_)
    : nodes_file_path{nodes_file_path_},
      roads_file_path{roads_file_path_},
      ways_file_path{ways_file_path_} {}

TxtMapParser::txt_parsed_t TxtMapParser::parse(void) {
  ifstream node_file((*this->nodes_file_path).c_str());
  node_file.exceptions(std::ofstream::failbit | std::ofstream::badbit);

  string node_str;
  if (node_file.is_open()) {
    int i = 0;
    while (std::getline(node_file, node_str) && i != 1000) {
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
      ++i;
    }
  }
  return this->parsed;
}
