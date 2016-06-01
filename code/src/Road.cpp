#include "Road.hpp"

Road::Road(long long int id_, unique_ptr<string> name_, bool is_two_way_)
    : id{id_} {
  if (name_ == nullptr) {
    this->name = std::make_unique<string>();
  } else {
    this->name = std::move(name_);
  }
  this->is_two_way = is_two_way_;
}

bool Road::operator==(const Road &otherRoad) const {
  return this->id == otherRoad.id;
}

long long int Road::getId() const { return this->id; }

string Road::getName() const { return *this->name; }

bool Road::isTwoWay() const { return this->is_two_way; }

ostream &operator<<(ostream &os, Road &road) {
  os << "ROAD id: " + std::to_string(road.id) + "\t name: " + *road.name +
            "\t two_way: " + ((road.is_two_way) ? "True" : "False");
  return os;
}

bool Road::addSubroad(Subroad *subroad) {
  if (subroad == nullptr) {
    return false;
  }
  this->subroads.push_back(subroad);
  return true;
}

vector<Subroad *> Road::getSubroads() {
  return this->subroads;
}
