/*
 * matcher.cpp
 */
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
using namespace std;

int editDistance(string pattern, string text);
float numApproximateStringMatching(string filename,string toSearch);

int editDistance(string pattern, string text)
{
  int n=text.length();
  vector<int> d(n+1);
  int old,neww;
  for (int j=0; j<=n; j++)
    d[j]=j;
  int m=pattern.length();
  for (int i=1; i<=m; i++) {
    old = d[0];
    d[0]=i;
    for (int j=1; j<=n; j++) {
      if (pattern[i-1]==text[j-1]) neww = old;
      else {
        neww = min(old,d[j]);
        neww = min(neww,d[j-1]);
        neww = neww +1;
      }
      old = d[j];
      d[j] = neww;
    }
  }
  return d[n];
}

float numApproximateStringMatching(string filename,string toSearch)
{
  ifstream fich(filename.c_str());
  if (!fich)
     { cout << "Erro a abrir ficheiro de leitura\n"; return 0; }

  string line1, word1;
  int num=0, nwords=0;

  while (!fich.eof()) {
    getline(fich,line1);
    stringstream s1(line1);
    while (!s1.eof()) {
      s1 >> word1;
      num += editDistance(toSearch,word1);
      nwords++;
    }
  }
  fich.close();
  float res=(float)num/nwords;
  return res;
}


