#include "StringAlgorithms.hpp"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
  cout << StringAlgorithms::naive("ola", "ola");
  cout << StringAlgorithms::naive("oldasdasdola", "ola");
  cout << StringAlgorithms::naive("ola", "oldasdasdola");
  cout << StringAlgorithms::naive("ola", "oaldasdasoladolda");
  cout << StringAlgorithms::naive("oaldasdasdoldaola", "ola") << endl;
  cout << StringAlgorithms::naiveAlternative("ola", "ola");
  cout << StringAlgorithms::naiveAlternative("oldasdasdola", "ola");
  cout << StringAlgorithms::naiveAlternative("ola", "oldasdasdola");
  cout << StringAlgorithms::naiveAlternative("ola", "oaldasdasoladolda");
  cout << StringAlgorithms::naiveAlternative("oaldasdasdoldaola", "ola") << endl;
  return 0;
}

// int numStringMatching(string filename, string toSearch) {
//   ifstream fich(filename.c_str());
//   if (!fich) {
//     cout << "Erro a abrir ficheiro de leitura\n";
//     return 0;
//   }
//
//   string line1;
//   int num = 0;
//
//   while (!fich.eof()) {
//     getline(fich, line1);
//     num += kmp(line1, toSearch);
//   }
//   fich.close();
//   return num;
// }
