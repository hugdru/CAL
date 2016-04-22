#include "TxtMapParser.hpp"

TxtMapParser::TxtMapParser(string const* const nodes_file_path_,
                           string const* const roads_file_path_,
                           string const* const ways_file_path_)
    : nodes_file_path{nodes_file_path_},
      roads_file_path{roads_file_path_},
      ways_file_path{ways_file_path_} {}
