#ifndef WebFetch_HPP_
#define WebFetch_HPP_

#include <string>

using namespace std;

class WebFetch {
private:
  string query_file;
  string output_file;
  string url;
public:
  enum class Returns {
	  FETCH_SUCCESS = 0,
	  FETCH_NO_OVERWRITE
  };
  void read_query_file();
  WebFetch(string query_file, string output_file);
  Returns fetch();
};

#endif /* WebFetch_HPP_ */
