#ifndef ROAD_HPP_
#define ROAD_HPP_

#include <string>
#include <memory>
#include <vector>


#include "Subroad.hpp"

using std::string;
using std::ostream;
using std::unique_ptr;
using std::vector;

class Road {
 private:
  const long long int id;
  unique_ptr<string> name;
  vector<Subroad *> subroads;
  bool is_two_way;

 public:
  Road(long long int id_, unique_ptr<string> name, bool is_two_way);
  bool operator==(const Road &otherRoad) const;
  friend ostream &operator<<(ostream &os, Road &road);
  long long int getId() const;
  string getName() const;
  bool isTwoWay() const;
  bool addSubroad(Subroad *subroad);
  vector<Subroad *> getSubroads();
};

#endif /* ROAD_HPP_ */
