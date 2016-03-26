#include <stdexcept>
#include <unordered_set>
#include <unordered_map>
#include <iostream>

#include "CommandLineParser.hpp"
#include "WebFetch.hpp"

static const string mapfile_long = "--mapfile";
static const string mapfile_short = "-mf";
static const string queryfile_long = "--queryfile";
static const string queryfile_short = "-qf";

static unordered_set<string> commandLineFlags {
	{mapfile_long},
	{mapfile_short},
	{queryfile_long},
	{queryfile_short}
};

CommandLineParser::CommandLineParser(const int cardinality, char **arguments) :
		cardinality { cardinality }, arguments { arguments } {
}

void CommandLineParser::parse(unordered_map<Options, string> &options) {

	options.clear();

	if (this->cardinality == 1) {
		error();
	}

	for (int index = 1; index < this->cardinality; ++index) {
		string commandLineFlag = this->arguments[index];
		auto iterator = commandLineFlags.find(commandLineFlag);
		if (iterator == commandLineFlags.end()) {
			error(commandLineFlag + " is not a valid option.\n");
		}
		if (commandLineFlag == mapfile_short || commandLineFlag == mapfile_long) {
			if (index + 1 >= this->cardinality) {
				error(commandLineFlag + " expecting one argument.\n");
			}
			options.insert({Options::MAP_FILE, this->arguments[++index]});
		} else if (commandLineFlag == queryfile_short || commandLineFlag == queryfile_long) {
			if (index + 1 >= this->cardinality) {
				error(commandLineFlag + " expecting one argument.\n");
			}
			options.insert({Options::QUERY_FILE, this->arguments[++index]});
		}
	}
}

void CommandLineParser::error(string error) {
	throw std::invalid_argument(error + "\n" + this->help);
}

void CommandLineParser::error() {
	throw std::invalid_argument(this->help);
}
