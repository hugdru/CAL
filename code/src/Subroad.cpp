#include "Subroad.hpp"

using std::to_string;

Subroad::Subroad(long long int id_road_, long long int node_src,
                 long long int node_dest)
    : id_road{id_road_},
      id_node_source{node_src},
      id_node_destination{node_dest} {}

bool Subroad::operator==(const Subroad &otherSubroad) const {
  return (this->id_road == otherSubroad.id_road) &&
         (this->id_node_source == otherSubroad.id_node_source) &&
         (this->id_node_destination == otherSubroad.id_node_destination);
}

void Subroad::getNodesId(Subroad::segment_t &segment) const {
  segment.source = this->id_node_source;
  segment.destination = this->id_node_destination;
}

long long int Subroad::getRoadId() const { return this->id_road; }

ostream &operator<<(ostream &os, Subroad &subroad) {
  os << "Subroad road: " + to_string(subroad.id_road) + "\t src_node: " +
            to_string(subroad.id_node_source) + "\t dest_node: " +
            to_string(subroad.id_node_destination);
  return os;
}
