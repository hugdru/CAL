#ifndef SUBROAD_HPP
#define SUBROAD_HPP

#include <ostream>

using std::ostream;

class Subroad {
 private:
  const long long int id_road;
  const long long int id_node1;
  const long long int id_node2;

 public:
  Subroad(long long int id_road_, long long int id_node1_, long long int id_node2_);
  bool operator==(const Subroad &otherSubroad) const;
  friend ostream &operator<<(ostream &os, Subroad &subroad);
};

#endif /* SUBROAD_HPP */
