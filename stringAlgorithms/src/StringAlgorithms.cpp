#include "StringAlgorithms.hpp"

int StringAlgorithms::naive(const string pattern, const string text) {
  int patternSize = pattern.size();
  int textSize = text.size();
  for (int textIndex = 0; textIndex <= (textSize - patternSize); ++textIndex) {
    if (text.compare(textIndex, patternSize, pattern) == 0) {
      return textIndex;
    }
  }
  return -1;
}

int StringAlgorithms::naiveAlternative(const string pattern, const string text) {
  int patternSize = pattern.size();
  int textSize = text.size();

  int textIndex, patternIndex;
  for (textIndex = 0, patternIndex = 0; textIndex < textSize && patternIndex < patternSize; ++textIndex) {
    if (pattern[patternIndex] == text[textIndex]) {
      ++patternIndex;
    } else {
      textIndex -= patternIndex;
      patternIndex = 0;
    }
  }
  if (patternIndex == patternSize) {
    return textIndex - patternIndex;
  }
  return -1;
}

int StringAlgorithms::knuthMorrisPratt(const string pattern, const string text) {
  int num = 0;
  int m = pattern.length();
  vector<int> prefix(m);
  knuthMorrisPrattBuildDFA(pattern, prefix);

  int n = text.length();

  int q = -1;
  for (int i = 0; i < n; i++) {
    while (q > -1 && pattern[q + 1] != text[i]) q = prefix[q];
    if (pattern[q + 1] == text[i]) q++;
    if (q == m - 1) {
      num++;
      q = prefix[q];
    }
  }
  return num;
}

void StringAlgorithms::knuthMorrisPrattBuildDFA(const string pattern, vector<int>& prefix) {
  int m = pattern.length();
  prefix[0] = -1;
  int k = -1;
  for (int q = 1; q < m; q++) {
    while (k > -1 && pattern[k + 1] != pattern[q]) k = prefix[k];
    if (pattern[k + 1] == pattern[q]) k = k + 1;
    prefix[q] = k;
  }
}
