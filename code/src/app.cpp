#include <iostream>

#include "WebFetch.hpp"
#include "CommandLineParser.hpp"
#include "app.hpp"
#include "QueryFileParser.hpp"

int main(int argc, char *argv[]) {
  using std::cout;
  using std::endl;

  CommandLineParser commandLineParser{argc, argv};
  unordered_map<CommandLineParser::Options, string> parsed_options{};

  try {
    commandLineParser.parse(parsed_options);
  } catch (const exception &e) {
    cerr << e.what() << endl;
    return EXIT_FAILURE;
  }

  auto end_it = parsed_options.end();
  auto query_it =
      parsed_options.find(CommandLineParser::Options::QUERY_FILE_PATH);
  auto map_it = parsed_options.find(CommandLineParser::Options::MAP_FILE_PATH);

  const string &map_file_path =
      (map_it != end_it) ? (*map_it).second : app::defaults::graph_file;

  if (query_it != end_it) {
    try {
      string &query_file_path = (*query_it).second;

      QueryFileParser queryFileParser{query_file_path};
      queryFileParser.parse();

      string url = queryFileParser.get<QueryFileParser::Field::URL>();
      cout << "url: " << url << "\n";

      WebFetch webfetch{url, map_file_path};

      if (webfetch.fetch() != WebFetch::Returns::FETCH_SUCCESS) {
        cerr << "Failed to fetch openstreetmap data.\n";
      }
    } catch (const exception &e) {
      cerr << e.what() << endl;
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
