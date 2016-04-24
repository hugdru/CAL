#ifndef ROAD_HPP_
#define ROAD_HPP_

#include <string>
#include <memory>

using std::string;
using std::ostream;
using std::unique_ptr;

class Road {
 private:
  const long long int id;
  unique_ptr<string> name;
  bool is_two_way;

 public:
  Road(long long int id_, unique_ptr<string> name, bool is_two_way);
  bool operator==(const Road &otherRoad) const;
  friend ostream &operator<<(ostream &os, Road &road);
};

#endif /* ROAD_HPP_ */
