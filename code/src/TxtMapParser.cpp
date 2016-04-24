#include "TxtMapParser.hpp"

#include <fstream>
#include <boost/algorithm/string.hpp>
#include <sstream>

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
  ifstream nodes_file((*this->nodes_file_path).c_str());
  nodes_file.exceptions(std::ifstream::badbit);

  if (nodes_file.is_open()) {
    string node_str;
    while (std::getline(nodes_file, node_str)) {
      vector<std::string> node_arguments;
      boost::split(node_arguments, node_str, boost::is_any_of(";"));
      if (node_arguments.size() != 5) {
        throw std::invalid_argument("A nodes file must have 5 arguments");
      }

      using std::stoll;
      using std::stod;
      long long int id = stoll(node_arguments[0]);
      double lat_deg = stod(node_arguments[1]);
      double lon_deg = stod(node_arguments[2]);
      double lat_rad = stod(node_arguments[3]);
      double lon_rad = stod(node_arguments[4]);
      this->parsed.nodes_umap.insert({id,
          shared_ptr<Node>(new Node{id, lat_deg, lon_deg, lat_rad, lon_rad})});
    }
  } else {
    throw std::invalid_argument("Nodes file invalid, failed to open file");
  }
}
void TxtMapParser::parse_roads(void) {
  ifstream roads_file((*this->roads_file_path).c_str());
  roads_file.exceptions(std::ifstream::badbit);

  if (roads_file.is_open()) {
    string road_str;
    while (std::getline(roads_file, road_str)) {
      vector<std::string> road_arguments;
      boost::split(road_arguments, road_str, boost::is_any_of(";"));
      auto road_arguments_size = road_arguments.size();
      if (road_arguments_size < 3) {
        throw std::invalid_argument("A roads file must have at least 3 arguments, " + road_arguments[0] + " has " + std::to_string(road_arguments.size()));
      }

      using std::stoll;
      long long int id = stoll(road_arguments[0]);

      std::stringstream road_names;

      int road_index_last = 1 + (int)road_arguments_size - 3;
      for (int road_index = 1; road_index <= road_index_last; ++road_index) {
        boost::trim(road_arguments[(size_t)road_index]);
        if (road_index + 1 <= road_index_last) {
          road_names << road_arguments[(size_t)road_index] << "; ";
        } else {
          road_names << road_arguments[(size_t)road_index];
        }
      }

      bool is_two_way;
      string is_two_way_read = road_arguments[road_arguments_size - 1];
      boost::trim(is_two_way_read);
      if (is_two_way_read == "True") {
        is_two_way = true;
      } else if (is_two_way_read == "False") {
        is_two_way = false;
      } else {
        throw std::invalid_argument(
            "A roads file third argument must be either True or False, " + road_arguments[0] + " is " + is_two_way_read);
      }

      unique_ptr<string> road_names_str (new string(road_names.str()));
      this->parsed.roads_umap.insert({id,
          shared_ptr<Road>(new Road{id, std::move(road_names_str), is_two_way})});
    }
  } else {
    throw std::invalid_argument("Roads file invalid, failed to open file");
  }
}

void TxtMapParser::parse_subroads(void) {
  ifstream subroads_file((*this->subroads_file_path).c_str());
  subroads_file.exceptions(std::ifstream::badbit);

  if (subroads_file.is_open()) {
    string subroad_str;
    while (std::getline(subroads_file, subroad_str)) {
      vector<std::string> subroad_arguments;
      boost::split(subroad_arguments, subroad_str, boost::is_any_of(";"));
      if (subroad_arguments.size() != 4) {
        throw std::invalid_argument("A subroads file must have 4 arguments");
      }

      using std::stoll;
      long long int id_road = stoll(subroad_arguments[0]);
      long long int id_node1 = stoll(subroad_arguments[1]);
      long long int id_node2 = stoll(subroad_arguments[2]);

      this->parsed.subroads_vector.push_back(
          shared_ptr<Subroad>(new Subroad{id_road, id_node1, id_node2}));
    }
  } else {
    throw std::invalid_argument("Subroads file invalid, failed to open file");
  }
}
