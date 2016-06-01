#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

#include "CommandLineParser.hpp"
#include "WebFetch.hpp"

static const string maptxtfiles_long = "--maptxtfiles";
static const string maptxtfiles_short = "-mtf";
static const string queryfile_long = "--queryfile";
static const string queryfile_short = "-qf";

static unordered_set<string> commandLineFlags{{maptxtfiles_long},
                                              {maptxtfiles_short},
                                              {queryfile_long},
                                              {queryfile_short}};

CommandLineParser::CommandLineParser(const int cardinality_, char **arguments_)
    : cardinality{cardinality_}, arguments{arguments_} {}

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

    if (commandLineFlag == maptxtfiles_short ||
        commandLineFlag == maptxtfiles_long) {
      if (index + 6 >= this->cardinality) {
        error(commandLineFlag + " expects 6 arguments.\n");
      }
      options.insert({Options::MAP_NODE_FILE_PATH, this->arguments[++index]});
      options.insert({Options::MAP_ROAD_FILE_PATH, this->arguments[++index]});
      options.insert(
          {Options::MAP_SUBROADS_FILE_PATH, this->arguments[++index]});
      options.insert({Options::MAP_START_NODE, this->arguments[++index]});
      options.insert({Options::MAP_GOAL_NODE, this->arguments[++index]});
      options.insert(
          {Options::MAP_SHORTEST_OVERPASS_FILE, this->arguments[++index]});

    } else if (commandLineFlag == queryfile_short ||
               commandLineFlag == queryfile_long) {
      if (index + 1 >= this->cardinality) {
        error(commandLineFlag + " expects one argument.\n");
      }
      options.insert({Options::QUERY_FILE_PATH, this->arguments[++index]});
      if (index + 1 < this->cardinality) {
        options.insert(
            {Options::QUERY_FILE_OUTPUT_PATH, this->arguments[++index]});
      }
    }
  }
}

void CommandLineParser::error(string error) {
  throw std::invalid_argument(error + "\n" + this->help);
}

void CommandLineParser::error() { throw std::invalid_argument(this->help); }
