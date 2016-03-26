#include <iostream>

#include "WebFetch.hpp"
#include "CommandLineParser.hpp"
#include "app.hpp"

int main(int argc, char *argv[]) {

	using std::cout;
	using std::endl;

	CommandLineParser commandLineParser {argc, argv};
	unordered_map<CommandLineParser::Options, string> parsed_options {};

	try {
		commandLineParser.parse(parsed_options);
	} catch (const exception &e) {
		cerr << e.what() << endl;
		return EXIT_FAILURE;
	}

	for (auto it = parsed_options.begin(); it != parsed_options.end(); ++it) {
		cerr << " " << (int)it->first << ":" << it->second << std::endl;
	}

	auto end_it = parsed_options.end();
	auto query_it = parsed_options.find(CommandLineParser::Options::QUERY_FILE);
	auto map_it = parsed_options.find(CommandLineParser::Options::MAP_FILE);
	const string &output_file = (map_it != end_it) ? (*map_it).second : app::defaults::graph_file;

	if (query_it != end_it) {
		try {
			WebFetch webfetch { (*query_it).second, output_file };
			webfetch.read_query_file();
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
