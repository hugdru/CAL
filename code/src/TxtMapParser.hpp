#include <string>

using std::string;

class TxtMapParser {
 private:
  string const* const nodes_file_path;
  string const* const roads_file_path;
  string const* const ways_file_path;

 public:
  TxtMapParser(string const* const nodes_file_path_,
               string const* const roads_file_path_,
               string const* const ways_file_path_);
};
