#include <string>
#include <vector>

using std::vector;
using std::string;

class StringAlgorithms {
  public:
    static int naive(const string pattern, const string text);
    static int naiveAlternative(const string pattern, const string text);
    static int knuthMorrisPratt(const string pattern, const string text);
  private:
    static void knuthMorrisPrattBuildDFA(const string pattern, vector<int>& prefix);
};
