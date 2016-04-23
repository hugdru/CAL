#include "Node.hpp"

using std::to_string;

Node::Node(long long int id_, double lat_deg, double lon_deg, double lat_rad,
           double lon_rad) {
  this->id = id_;
  this->coordinates_degrees.latitude = lat_deg;
  this->coordinates_degrees.longitude = lon_deg;
  this->coordinates_radians.latitude = lat_rad;
  this->coordinates_radians.longitude = lon_rad;
}

bool Node::operator==(const Node &otherNode) const {
  return this->id == otherNode.id;
}

ostream &operator<<(ostream &os, Node &node) {
  os << "NODE id: " + to_string(node.id) + "\t degrees: (" +
            to_string(node.coordinates_degrees.latitude) + ";" +
            to_string(node.coordinates_degrees.longitude) + ")\t radians: (" +
            to_string(node.coordinates_radians.latitude) + ";" +
            to_string(node.coordinates_radians.longitude) + ")";
  return os;
}
