#ifndef NODE_HPP_
#define NODE_HPP_

#include <string>
#include <utility>

using std::string;
using std::ostream;
using std::pair;

class Node {
 private:
  using coordinates_t = struct {
    double latitude;
    double longitude;
  };
  const long long int id;
  coordinates_t coordinates_degrees;
  coordinates_t coordinates_radians;

 public:
  Node(long long int id_, double lat_deg, double lon_deg, double lat_rad,
       double lon_rad);
  bool operator==(const Node &otherNode) const;
  double distance(const Node &otherNode) const;
  bool operator<(const Node &otherNode) const;
  long long int getId() const;
  friend ostream &operator<<(ostream &os, Node &node);
};

#endif /* NODE_HPP_ */
