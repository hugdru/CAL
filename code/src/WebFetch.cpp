#include "WebFetch.hpp"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include <boost/filesystem.hpp>

WebFetch::WebFetch(string url_, string output_file_)
    : url{url_}, output_file{output_file_} {
  if (url.empty() || output_file.empty()) {
    throw std::invalid_argument(
        "WebFetch expecting a valid: url, output_file.");
  }
}

WebFetch::Returns WebFetch::fetch() {
  char const* const output_file_cstring = this->output_file.c_str();

  if (boost::filesystem::exists(output_file_cstring)) {
    cout << this->output_file << " already exists, type Yy to overwrite."
         << endl;
    char input[3] = {0};
    cin.get(input, 3);
    if (strncmp(input, "Yy", 2) != 0) {
      return WebFetch::Returns::FETCH_NO_OVERWRITE;
    }
  }

  ofstream file;
  file.exceptions(std::ofstream::failbit | std::ofstream::badbit);

  file.open(output_file_cstring, ios_base::out | ios_base::trunc);
  if (file.is_open()) {
    file << curlpp::options::Url(this->url);
    file.close();
  }

  return WebFetch::Returns::FETCH_SUCCESS;
}
