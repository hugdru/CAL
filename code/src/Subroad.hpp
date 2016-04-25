#ifndef SUBROAD_HPP
#define SUBROAD_HPP

#include <ostream>

using std::ostream;

class Subroad {
 private:
  const long long int id_road;
  const long long int id_node_source;
  const long long int id_node_destination;

 public:
  using segment_t = struct {
    long long int source;
    long long int destination;
  };
  Subroad(long long int id_road_, long long int node_src,
          long long int node_dest);
  bool operator==(const Subroad &otherSubroad) const;
  friend ostream &operator<<(ostream &os, Subroad &subroad);
  long long int getRoad() const;
  void getNodesId(Subroad::segment_t &) const;
  long long int getRoadId() const;
};

#endif /* SUBROAD_HPP */
