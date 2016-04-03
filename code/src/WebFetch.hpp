#ifndef WebFetch_HPP_
#define WebFetch_HPP_

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

#endif /* WebFetch_HPP_ */
