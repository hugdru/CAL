#include "Subroad.hpp"

using std::to_string;

Subroad::Subroad(long long int id_road_, long long int id_node1_, long long int id_node2_) : id_road {id_road_}, id_node1{id_node1_}, id_node2{id_node2_} {}

bool Subroad::operator==(const Subroad &otherSubroad) const {
  return
    (this->id_road == otherSubroad.id_road) &&
    (this->id_node1 == otherSubroad.id_node1) &&
    (this->id_node2 == otherSubroad.id_node2);
}

ostream &operator<<(ostream &os, Subroad &subroad) {
  os << "Subroad road_id: " + to_string(subroad.id_road) + "\t id_node1: " +
            to_string(subroad.id_node1) + "\t id_node2: " +
            to_string(subroad.id_node2);
  return os;
}
