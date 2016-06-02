#include "StringAlgorithms.hpp"

int StringAlgorithms::naive(const string pattern, const string text) {
  int patternLength = (int)pattern.length();
  int textLength = (int)text.length();

  if (textLength < patternLength) {
    return -1;
  }

  for (int textIndex = 0; textIndex <= (textLength - patternLength);
       ++textIndex) {
    if (text.compare((size_t)textIndex, (size_t)patternLength, pattern) == 0) {
      return textIndex;
    }
  }
  return -1;
}

int StringAlgorithms::naiveAlternative(const string pattern,
                                       const string text) {
  int patternLength = (int)pattern.size();
  int textLength = (int)text.size();

  if (textLength < patternLength) {
    return -1;
  }

  int textIndex, patternIndex;
  for (textIndex = 0, patternIndex = 0;
       textIndex < textLength && patternIndex < patternLength; ++textIndex) {
    if (pattern[(size_t)patternIndex] == text[(size_t)textIndex]) {
      ++patternIndex;
    } else {
      textIndex -= patternIndex;
      patternIndex = 0;
    }
  }
  if (patternIndex == patternLength) {
    return textIndex - patternIndex;
  }
  return -1;
}
int StringAlgorithms::knuthMorrisPratt(const string pattern,
                                       const string text) {
  vector<int> *piTable = knuthMorrisPrattBuildPiTable(pattern);

  if (piTable == nullptr) {
    return -1;
  }
  return knuthMorrisPratt(pattern, text, piTable);
}

int StringAlgorithms::knuthMorrisPratt(const string pattern, const string text, vector<int> *piTable) {
  int patternLength = (int)pattern.length();
  int textLength = (int)text.length();

  if (textLength < patternLength) {
    return -1;
  }

  if ((int)(*piTable).size() != patternLength) {
    return -1;
  }

  int nCharsMatched = -1;
  for (int i = 0; i < textLength; ++i) {
    while (nCharsMatched > -1 &&
           pattern[(size_t)nCharsMatched + 1] != text[(size_t)i]) {
      nCharsMatched = (*piTable)[(size_t)nCharsMatched];
    }
    if (pattern[(size_t)nCharsMatched + 1] == text[(size_t)i]) {
      ++nCharsMatched;
    }
    if (nCharsMatched == patternLength - 1) {
      return i - patternLength + 1;
    }
  }
  return -1;
}

vector<int>* StringAlgorithms::knuthMorrisPrattBuildPiTable(
    const string pattern) {
  int patternLength = (int)pattern.length();
  if (pattern.empty()) {
    return nullptr;
  }
  vector<int> *piTable = new vector<int>((size_t)patternLength);

  (*piTable)[0] = -1;
  int k = -1;
  for (int i = 1; i <= patternLength; i++) {
    while (k >= 0 && pattern[(size_t)k + 1] != pattern[(size_t)i]) {
      k = (*piTable)[(size_t)k];
    }
    (*piTable)[(size_t)i] = ++k;
  }

  return piTable;
}

int StringAlgorithms::dynamicProgrammingLevenshteinDistance(
    const string pattern, const string text) {
  int textLength = (int)text.length();
  vector<int> distance((size_t)textLength + 1);
  for (int i = 0; i <= textLength; ++i) {
    distance[(size_t)i] = i;
  }
  int patternLength = (int)pattern.length();
  for (int i = 1; i <= patternLength; ++i) {
    int oldVal = distance[0];
    distance[0] = i;
    for (int j = 1; j <= textLength; ++j) {
      int newVal = oldVal;
      if (pattern[(size_t)i - 1] != text[(size_t)j - 1]) {
        newVal = std::min(oldVal, distance[(size_t)j]);
        newVal = std::min(newVal, distance[(size_t)j - 1]);
        newVal = newVal + 1;
      }
      oldVal = distance[(size_t)j];
      distance[(size_t)j] = newVal;
    }
  }
  return distance[(size_t)textLength];
}
