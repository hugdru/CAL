#ifndef WEBFETCH_HPP_
#define WEBFETCH_HPP_

#include <string>

using namespace std;

class WebFetch {
 private:
  string url;
  string output_file;

 public:
  enum class Returns { FETCH_SUCCESS = 0, FETCH_NO_OVERWRITE };
  WebFetch(string url, string output_file);
  Returns fetch();
};

#endif /* WEBFETCH_HPP_ */
