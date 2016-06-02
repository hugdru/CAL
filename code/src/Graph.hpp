#ifndef GRAPH_HPP_
#define GRAPH_HPP_

#include <vector>
#include <queue>
#include <set>
#include <list>

#include "Subroad.hpp"

using namespace std;

template <typename T>
class Edge;
template <typename T>
class Graph;

/* VERTEX */
template <typename T>
class Vertex {
 public:
  using adjacency_list_t = vector<Edge<T>>;

 private:
  T info;
  adjacency_list_t adjacency_list;
  bool visited;
  double cost;
  Edge<T>* cost_edge;

 public:
  friend class Graph<T>;

  Vertex(T in);
  bool operator<(const Vertex<T>& vertex) const;
  bool operator==(const Vertex<T>& vertex) const;

  void addEdge(Vertex<T>* dest, double weight, Subroad* subroad_);
  bool removeEdgeTo(Vertex<T> const* const otherVertex);

  T getInfo() const;
  double getCost() const;
  typename Vertex<T>::adjacency_list_t getAdjacencyList() const;
  Vertex<T>* getPath() const;
};

template <typename T>
bool Vertex<T>::operator<(const Vertex<T>& vertex) const {
  return this->info < vertex.info;
}

template <typename T>
bool Vertex<T>::operator==(const Vertex<T>& vertex) const {
  return this->info == vertex.info;
}

template <typename T>
struct VertexCostGreaterThan {
  bool operator()(Vertex<T> const* const lVertex,
                  Vertex<T> const* const rVertex) const {
    return lVertex->getCost() > rVertex->getCost();
  }
};

template <typename T>
bool Vertex<T>::removeEdgeTo(Vertex<T> const* const otherVertex) {
  for (auto it_edge = this->adjacency_list.begin();
       it_edge != this->adjacency_list.end(); ++it_edge) {
    if (it_edge->target == otherVertex) {
      this->adjacency_list.erase(it_edge);
      return true;
    }
  }
  return false;
}

template <typename T>
Vertex<T>::Vertex(T in)
    : info{in}, visited{false}, cost{0}, cost_edge{NULL} {}

template <typename T>
void Vertex<T>::addEdge(Vertex<T>* target, double weight, Subroad* subroad_) {
  Edge<T> new_edge{this, target, weight, subroad_};
  this->adjacency_list.push_back(new_edge);
}

template <typename T>
T Vertex<T>::getInfo() const {
  return this->info;
}

template <typename T>
double Vertex<T>::getCost() const {
  return this->cost;
}

template <typename T>
typename Vertex<T>::adjacency_list_t Vertex<T>::getAdjacencyList() const {
  return this->adjacency_list;
}

template <typename T>
Vertex<T>* Vertex<T>::getPath() const {
  return this->cost_edge;
}

/* EDGE */
template <typename T>
class Edge {
 private:
  Vertex<T>* from;
  Vertex<T>* target;
  double weight;
  Subroad* subroad;

 public:
  friend class Graph<T>;
  friend class Vertex<T>;

  Edge(Vertex<T>* from_, Vertex<T>* target_, double weight_,
       Subroad* subdroad_);
  bool operator<(const Edge<T>& otherEdge) const;

  double getWeight() const;
  Vertex<T>* getTarget() const;
};

template <typename T>
Edge<T>::Edge(Vertex<T>* from_, Vertex<T>* target_, double weight_,
              Subroad* subroad_)
    : from{from_}, target{target_}, weight{weight_}, subroad{subroad_} {}

template <typename T>
double Edge<T>::getWeight() const {
  return this->weight;
}

template <typename T>
Vertex<T>* Edge<T>::getTarget() const {
  return this->target;
}

template <typename T>
bool Edge<T>::operator<(const Edge<T>& otherEdge) const {
  return this->weight < otherEdge.weight;
}

template <typename T>
struct VertexSetGreaterThan {
  bool operator()(Vertex<T> const* const lhs,
                  Vertex<T> const* const rhs) const {
    return (*lhs < *rhs);
  }
};

/* GRAPH */
template <typename T>
class Graph {
 public:
  using VertexSet_t = set<Vertex<T>*, VertexSetGreaterThan<T>>;

 private:
  VertexSet_t vertexSet;
  long unsigned int edge_count = 0;
  int numCycles;

  void dfs(Vertex<T>* v, vector<T>& path_history) const;

 public:
  vector<T> dfs() const;
  vector<T> bfs(Vertex<T>* v) const;
  void dijkstra(const T& in);
  void getNodesPathTo(const T& in, list<T>& res);
  void getSubroadsPathTo(const T& in, list<Subroad*>& subroads_path);

  bool addVertex(const T& in);
  bool addEdge(const T& in_origin, const T& in_target, double weight,
               Subroad* subroad_);
  bool removeVertex(const T& in);
  bool removeEdge(const T& in_origin, const T& in_target);

  typename Graph<T>::VertexSet_t getVertexSet() const;
  long unsigned int getVertexCount() const;
  long unsigned int getEdgeCount() const;
  Vertex<T>* getVertex(
      const T& in, typename Graph<T>::VertexSet_t::iterator* it_ptr = nullptr,
      bool erase = false);
  vector<Vertex<T>*> getSources() const;
  void uniform_cost_search(const T& in, const T& goal);
  ~Graph();
};

template <typename T>
long unsigned int Graph<T>::getVertexCount() const {
  return vertexSet.size();
}
template <typename T>
long unsigned int Graph<T>::getEdgeCount() const {
  return this->edge_count;
}
template <typename T>
typename Graph<T>::VertexSet_t Graph<T>::getVertexSet() const {
  return vertexSet;
}

template <typename T>
bool Graph<T>::addVertex(const T& in) {
  Vertex<T>* new_vertex = new Vertex<T>(in);
  auto pair = this->vertexSet.insert(new_vertex);
  if (!pair.second) {
    delete new_vertex;
    return false;
  }
  return true;
}

template <typename T>
Graph<T>::~Graph() {
  for (auto &vertex : vertexSet) {
    delete vertex;
  }
}

template <typename T>
bool Graph<T>::removeVertex(const T& in) {
  Vertex<T>* vertex_info = this->getVertex(in, nullptr, true);
  if (vertex_info == nullptr) {
    return false;
  }

  for (const auto& vertex_ptr : this->vertexSet) {
    vertex_ptr->removeEdgeTo(vertex_info);
  }

  delete vertex_info;
  return true;
}

template <typename T>
bool Graph<T>::addEdge(const T& in_origin, const T& in_target, double weight,
                       Subroad* subroad_) {
  Vertex<T>* vertex_origin = this->getVertex(in_origin);
  Vertex<T>* vertex_target = this->getVertex(in_target);
  if (vertex_origin == nullptr || vertex_target == nullptr) {
    return false;
  }

  vertex_origin->addEdge(vertex_target, weight, subroad_);

  this->edge_count++;
  return true;
}

template <typename T>
bool Graph<T>::removeEdge(const T& in_origin, const T& in_target) {
  Vertex<T>* vertex_origin = this->getVertex(in_origin);
  Vertex<T>* vertex_target = this->getVertex(in_target);
  if (vertex_origin == nullptr || vertex_target == nullptr) {
    return false;
  }

  if (!vertex_origin->removeEdgeTo(vertex_target)) {
    return false;
  }

  this->edge_count--;
  return true;
}

template <typename T>
vector<T> Graph<T>::dfs() const {
  for (auto& vertex_ptr : this->vertexSet) {
    vertex_ptr->visited = false;
  }

  vector<T> path_history;
  for (auto vertex_ptr : this->vertexSet) {
    if (!vertex_ptr->visited) {
      dfs(vertex_ptr, path_history);
    }
  }

  return path_history;
}

template <typename T>
void Graph<T>::dfs(Vertex<T>* vertex, vector<T>& path_history) const {
  vertex->visited = true;
  path_history.push_back(vertex->info);

  for (auto& edge : this->adjacency_list) {
    if (edge.target->visited == false) {
      dfs(edge.target, path_history);
    }
  }
}

template <typename T>
vector<T> Graph<T>::bfs(Vertex<T>* vertex) const {
  vector<T> path_history;
  queue<Vertex<T>*> fringe;
  fringe.push(vertex);
  vertex->visited = true;

  while (!fringe.empty()) {
    Vertex<T>* selected_vertex = fringe.front();
    fringe.pop();
    path_history.push_back(selected_vertex->info);

    for (auto& edge : this->adjacency_list) {
      Vertex<T>* candidate_vertex = edge.target;
      if (!candidate_vertex->visited) {
        candidate_vertex->visited = true;
        fringe.push(candidate_vertex);
      }
    }
  }
  return path_history;
}

template <typename T>
Vertex<T>* Graph<T>::getVertex(const T& in,
                               typename Graph<T>::VertexSet_t::iterator* it_ptr,
                               bool erase) {
  auto vertex_in_search = make_shared<Vertex<T>>(in);

  auto it_vertex_info = this->vertexSet.find(vertex_in_search.get());
  if (it_vertex_info == this->vertexSet.end()) {
    if (it_ptr != nullptr) {
      *it_ptr = this->vertexSet.end();
    }
    return NULL;
  }

  if (it_ptr != nullptr) {
    *it_ptr = it_vertex_info;
  }

  Vertex<T>* vertex_ptr = *it_vertex_info;
  if (erase) {
    if (it_ptr != nullptr) {
      *it_ptr = this->vertexSet.end();
    }
    this->vertexSet.erase(it_vertex_info);
  }
  return vertex_ptr;
}

template <typename T>
void Graph<T>::dijkstra(const T& in) {
  for (auto& vertex_ptr : this->vertexSet) {
    vertex_ptr->cost_edge = NULL;
    vertex_ptr->cost = std::numeric_limits<double>::max();
    vertex_ptr->visited = false;
  }

  Vertex<T>* selected_vertex = this->getVertex(in);
  if (selected_vertex == nullptr) {
    return;
  }
  selected_vertex->cost = 0;

  vector<Vertex<T>*> priority_queue;
  priority_queue.push_back(selected_vertex);

  make_heap(priority_queue.begin(), priority_queue.end());

  while (!priority_queue.empty()) {
    selected_vertex = priority_queue.front();
    pop_heap(priority_queue.begin(), priority_queue.end());
    priority_queue.pop_back();

    for (auto& edge : selected_vertex->adjacency_list) {
      Vertex<T>* target = edge.target;

      double new_target_cost = selected_vertex->cost + edge.weight;
      if (new_target_cost < target->cost) {
        target->cost = new_target_cost;
        target->cost_edge = &edge;

        if (!target->visited) {
          target->visited = true;
          priority_queue.push_back(target);
        }

        make_heap(priority_queue.begin(), priority_queue.end(),
                  VertexCostGreaterThan<T>());
      }
    }
  }
}

template <typename T>
void Graph<T>::getNodesPathTo(const T& in, list<T>& path) {
  path.clear();

  Vertex<T>* goal_vertex = this->getVertex(in);
  if (goal_vertex == nullptr) {
    return;
  }

  path.push_front(goal_vertex->info);
  Edge<T>* cost_edge = goal_vertex->cost_edge;
  while (cost_edge != nullptr) {
    path.push_front(cost_edge->from->info);
    cost_edge = cost_edge->from->cost_edge;
  }
}

template <typename T>
void Graph<T>::getSubroadsPathTo(const T& in, list<Subroad*>& subroads_path) {
  subroads_path.clear();

  Vertex<T>* goal_vertex = this->getVertex(in);
  if (goal_vertex == nullptr) {
    return;
  }

  Edge<T>* cost_edge = goal_vertex->cost_edge;
  while (cost_edge != nullptr) {
    subroads_path.push_front(cost_edge->subroad);
    cost_edge = cost_edge->from->cost_edge;
  }
}

#endif /* GRAPH_HPP_ */
