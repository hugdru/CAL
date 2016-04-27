#include <iostream>
#include <boost/filesystem.hpp>

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

static string generate_overpass_map_query(list<Subroad *> &subroads_list);

static void save_overpass_map_query(string &map_shortest_result,
                                    string &file_path_out);

int main(int argc, char *argv[]) {
  using std::cout;
  using std::endl;

  CommandLineParser commandLineParser{argc, argv};
  unordered_map<CommandLineParser::Options, string> parsed_options{};

  try {
    cout << "Parsing commandLine options" << endl;
    commandLineParser.parse(parsed_options);
  } catch (const exception &e) {
    cerr << e.what() << endl;
    return EXIT_FAILURE;
  }

  cout << "Analyzing commandLine options" << endl;
  unique_ptr<TxtMapParser> txtFileParser =
      CommandLineParserAnalyzer(parsed_options);
  if (txtFileParser == nullptr) {
    return EXIT_SUCCESS;
  }

  cout << "Parsing nodes, roads and subroads files" << endl;
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

  cout << "Building graph" << endl;
  auto graph_ptr = buildGraph(parsed_txt);

  long long int id_node_start = stoll(parsed_options.at(
      CommandLineParser::Options::MAP_START_NODE));
  long long int id_node_goal = stoll(parsed_options.at(
      CommandLineParser::Options::MAP_GOAL_NODE));

  bool valid_start_goal = true;
  auto it_end_nodes_umap = parsed_txt.nodes_umap.end();
  auto it_start_node = parsed_txt.nodes_umap.find(id_node_start);
  if (it_start_node == it_end_nodes_umap) {
    valid_start_goal = false;
    cerr << "invalid start_node_id: " << id_node_start << endl;
  }
  auto it_goal_node = parsed_txt.nodes_umap.find(id_node_goal);
  if (it_goal_node == it_end_nodes_umap) {
    valid_start_goal = false;
    cerr << "invalid goal_node_id: " << id_node_goal << endl;
  }

  if (!valid_start_goal) {
    exit(EXIT_FAILURE);
  }

  Node const *start_node_ptr = it_start_node->second;
  Node const *goal_node_ptr = it_goal_node->second;

  cout << endl
       << "Running dijkstra, start:" + to_string(id_node_start) + "; goal:" +
              to_string(id_node_goal)
       << endl;

  graph_ptr->dijkstra(*start_node_ptr);
  list<Node> nodes_to_goal;

  list<Subroad *> subroads_to_goal;
  graph_ptr->getSubroadsPathTo(*goal_node_ptr, subroads_to_goal);
  string map_result = generate_overpass_map_query(subroads_to_goal);
  cout << endl
       << "Go to: http://overpass-turbo.eu/ ; paste the following ; type run ; "
          "click the magnifying class (zoom to data)"
       << endl;
  cout << map_result << endl;

  save_overpass_map_query(
      map_result, parsed_options.at(
                      CommandLineParser::Options::MAP_SHORTEST_OVERPASS_FILE));

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
      graph_ptr->addEdge(*node_src, *node_dest, distance, subroad);
      if (road->isTwoWay()) {
        graph_ptr->addEdge(*node_dest, *node_src, distance, subroad);
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

static string generate_overpass_map_query(list<Subroad *> &subroads_list) {
  static stringstream url;
  url << "(";
  for (auto &subroad_ptr : subroads_list) {
    Subroad::segment_t node_ids;
    subroad_ptr->getNodesId(node_ids);
    url << "node(" << node_ids.source << ");"
        << "node(" << node_ids.destination << ");"
        << "way(" << subroad_ptr->getRoadId() << ");";
  }
  url << ");out;";
  return url.str();
}

static void save_overpass_map_query(string &map_shortest_result,
                                    string &file_path_out) {
  char const *const file_path_out_cstring = file_path_out.c_str();

  if (boost::filesystem::exists(file_path_out_cstring)) {
    cout << endl
         << file_path_out << " already exists, type Yy to overwrite." << endl;
    char input[3] = {0};
    cin.get(input, 3);
    if (strncmp(input, "Yy", 2) != 0) {
      return;
    }
  }

  std::ofstream file;
  file.exceptions(std::ofstream::failbit | std::ofstream::badbit);

  file.open(file_path_out_cstring, ios_base::out | ios_base::trunc);
  if (file.is_open()) {
    file << map_shortest_result;
    file.close();
  }
}
