#include <iostream>

#include "WebFetch.hpp"
#include "CommandLineParser.hpp"
#include "app.hpp"
#include "QueryFileParser.hpp"
#include "TxtMapParser.hpp"

static unique_ptr<TxtMapParser> CommandLineParserAnalyzer(
    unordered_map<CommandLineParser::Options, string> &parsed_options);

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

  unique_ptr<TxtMapParser> txtFileParser =
      CommandLineParserAnalyzer(parsed_options);
  if (txtFileParser == nullptr) {
    return EXIT_SUCCESS;
  }

  TxtMapParser::txt_parsed_t parsed = txtFileParser->parse();
  for (Node *node_ptr : parsed.node_vector) {
    std::cout << *node_ptr << std::endl;
  }

  return EXIT_SUCCESS;
}

static unique_ptr<TxtMapParser> CommandLineParserAnalyzer(
    unordered_map<CommandLineParser::Options, string> &parsed_options) {
  auto end_it = parsed_options.end();
  auto query_it =
      parsed_options.find(CommandLineParser::Options::QUERY_FILE_PATH);
  auto query_output_it =
      parsed_options.find(CommandLineParser::Options::QUERY_FILE_OUTPUT_PATH);
  auto map_nodes_it =
      parsed_options.find(CommandLineParser::Options::MAP_NODE_FILE_PATH);
  auto map_roads_it =
      parsed_options.find(CommandLineParser::Options::MAP_ROAD_FILE_PATH);
  auto map_subroads_it =
      parsed_options.find(CommandLineParser::Options::MAP_SUBROADS_FILE_PATH);

  const string &map_file_path = (query_output_it != end_it)
                                    ? (*query_output_it).second
                                    : app::defaults::graph_file;

  if (query_it != end_it) {
    try {
      string &query_file_path = (*query_it).second;

      QueryFileParser queryFileParser{query_file_path};
      queryFileParser.parse();

      string url = queryFileParser.get<QueryFileParser::Field::URL>();
      cout << "url: " << url << "\n";

      WebFetch webfetch{url, map_file_path};

      if (webfetch.fetch() != WebFetch::Returns::FETCH_SUCCESS) {
        cerr << "Failed to fetch data.\n";
      }
    } catch (const exception &e) {
      cerr << e.what() << endl;
      exit(EXIT_FAILURE);
    }

    return nullptr;
  }

  if (map_nodes_it != end_it) {
    return unique_ptr<TxtMapParser>(new TxtMapParser{
        &map_nodes_it->second, &map_roads_it->second, &map_subroads_it->second});
  }
  return nullptr;
}
