#include <string>
#include <vector>
#include <memory>

using std::vector;
using std::string;
using std::unique_ptr;
using std::make_unique;

class StringAlgorithms {
  public:
    static int naive(const string pattern, const string text);
    static int naiveAlternative(const string pattern, const string text);
    static int knuthMorrisPratt(const string pattern, const string text);
    static int knuthMorrisPratt(const string pattern, const string text, unique_ptr<vector<int>> const &piTable);
    static unique_ptr<vector<int>> knuthMorrisPrattBuildPiTable(const string pattern);
    static int dynamicProgrammingLevenshteinDistance(const string pattern, const string text);
};
