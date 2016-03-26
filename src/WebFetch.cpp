#include "WebFetch.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

using boost::property_tree::ptree;

WebFetch::WebFetch(string query_file, string output_file) :
		query_file { query_file }, output_file { output_file } {
	if (query_file.empty() || output_file.empty()) {
		throw std::invalid_argument(
				"WebFetch expecting a valid: query_file, output_file.");
	}
}

WebFetch::Returns WebFetch::fetch() {

	char const * const output_file_cstring = this->output_file.c_str();

	if (boost::filesystem::exists(output_file_cstring)) {
		cout << this->output_file << " already exists, type Yy to overwrite."
				<< endl;
		char input[3] = { 0 };
		cin.get(input, 3);
		if (strncmp(input, "Yy", 2) != 0) {
			return WebFetch::Returns::FETCH_NO_OVERWRITE;
		}
	}

	ofstream file;
	file.exceptions(std::ofstream::failbit | std::ofstream::badbit);

	file.open(output_file_cstring, ios_base::out | ios_base::trunc);
	if (file.is_open()) {
		file << curlpp::options::Url(this->url);
		file.close();
	}

	return WebFetch::Returns::FETCH_SUCCESS;
}

void WebFetch::read_query_file() {

	ifstream file { this->query_file.c_str() };
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	if (file.is_open()) {
		ptree treePointer;
		read_json(file, treePointer);

		string holder;

		url = treePointer.get<string>("location")
				+ treePointer.get<string>("query");

		file.close();
	}
}
