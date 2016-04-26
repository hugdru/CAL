#include "Node.hpp"

#include <cmath>

using std::to_string;

Node::Node(long long int id_, double lat_deg, double lon_deg, double lat_rad,
           double lon_rad)
    : id{id_} {
  this->coordinates_degrees.latitude = lat_deg;
  this->coordinates_degrees.longitude = lon_deg;
  this->coordinates_radians.latitude = lat_rad;
  this->coordinates_radians.longitude = lon_rad;
}

bool Node::operator==(const Node &otherNode) const {
  return this->id == otherNode.id;
}

double Node::distance(const Node &otherNode) const {
  using std::abs;
  // static const long long int earth_radius = 6.3685E6;
  static const long long int earth_radius = 1;
  double delta_latitude = abs(this->coordinates_radians.latitude -
                              otherNode.coordinates_radians.latitude);
  double delta_longitude = abs(this->coordinates_radians.longitude -
                               otherNode.coordinates_radians.longitude);

  double perimeter_latitude = delta_latitude * earth_radius;
  double perimeter_longitude = delta_longitude * earth_radius;

  return std::sqrt(perimeter_latitude * perimeter_longitude +
                   perimeter_longitude * perimeter_longitude);
}

long long int Node::getId() const {
  return this->id;
}

bool Node::operator<(const Node &otherNode) const {
  return this->id < otherNode.id;
}

ostream &operator<<(ostream &os, Node &node) {
  os << "NODE id: " + to_string(node.id) + "\t degrees: (" +
            to_string(node.coordinates_degrees.latitude) + ";" +
            to_string(node.coordinates_degrees.longitude) + ")\t radians: (" +
            to_string(node.coordinates_radians.latitude) + ";" +
            to_string(node.coordinates_radians.longitude) + ")";
  return os;
}
