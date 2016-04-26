#include <iostream>

#include "WebFetch.hpp"
#include "CommandLineParser.hpp"
#include "app.hpp"
#include "QueryFileParser.hpp"
#include "TxtMapParser.hpp"
#include "Graph.hpp"

static unique_ptr<TxtMapParser> CommandLineParserAnalyzer(
    unordered_map<CommandLineParser::Options, string> &parsed_options);

static unique_ptr<Graph<Node>> buildGraph(
    TxtMapParser::txt_parsed_t &parsed_txt);

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

  TxtMapParser::txt_parsed_t parsed_txt = txtFileParser->parse();
  for (const auto &node_ptr : parsed_txt.nodes_umap) {
    cout << *(node_ptr.second) << endl;
  }
  for (const auto &road_ptr : parsed_txt.roads_umap) {
    cout << *(road_ptr.second) << endl;
  }
  for (const auto &subroad_ptr : parsed_txt.subroads_vector) {
    cout << *subroad_ptr << endl;
  }

  auto graph_ptr = buildGraph(parsed_txt);

  long long int id_node_start = 25503962;
  long long int id_node_goal = 4097873936;

  Node const *const start_node_ptr = parsed_txt.nodes_umap.at(id_node_start);
  Node const *const goal_node_ptr = parsed_txt.nodes_umap.at(id_node_goal);

  cout << endl << "Running dijkstra, start:" + to_string(id_node_start) + "; goal:" + to_string(id_node_goal) << endl;

  graph_ptr->dijkstra(*start_node_ptr);
  list<Node> path_to_goal;
  graph_ptr->getPathTo(*goal_node_ptr, path_to_goal);

  auto it_node_end = path_to_goal.end();
  for(auto it_node = path_to_goal.cbegin(); it_node != it_node_end; ++it_node) {
    string id_to_string = to_string(it_node->getId()); 
    if (next(it_node) == it_node_end) {
      cout << id_to_string << endl;
    } else {
      cout << id_to_string + "->";
    }
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
    return std::make_unique<TxtMapParser>(
        &map_nodes_it->second, &map_roads_it->second, &map_subroads_it->second);
  }
  return nullptr;
}

static unique_ptr<Graph<Node>> buildGraph(
    TxtMapParser::txt_parsed_t &parsed_txt) {
  unique_ptr<Graph<Node>> graph_ptr = make_unique<Graph<Node>>();

  for (const auto &node_ptr : parsed_txt.nodes_umap) {
    graph_ptr->addVertex(*node_ptr.second);
  }
  cout << endl
       << "Created " << graph_ptr->getVertexCount() << " vertices" << endl;

  for (const auto &subroad : parsed_txt.subroads_vector) {
    auto road_id = subroad->getRoadId();
    Subroad::segment_t road_nodes_id;
    subroad->getNodesId(road_nodes_id);

    Road const *const road = parsed_txt.roads_umap.at(road_id);
    try {
      Node const *const node_src =
          parsed_txt.nodes_umap.at(road_nodes_id.source);
      Node const *const node_dest =
          parsed_txt.nodes_umap.at(road_nodes_id.destination);
      auto distance = node_src->distance(*node_dest);
      graph_ptr->addEdge(*node_src, *node_dest, distance);
      if (road->isTwoWay()) {
        graph_ptr->addEdge(*node_dest, *node_src, distance);
      }
    } catch (const logic_error &e) {
      cerr << e.what() << " " << road_nodes_id.source << " or "
           << road_nodes_id.destination << " missing.";
      exit(EXIT_FAILURE);
    }
  }
  cout << "Created " << graph_ptr->getEdgeCount() << " edges" << endl;

  return graph_ptr;
}
