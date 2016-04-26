#ifndef GRAPH_HPP_
#define GRAPH_HPP_

#include <vector>
#include <queue>
#include <set>

using namespace std;

template <typename T>
class Edge;
template <typename T>
class Graph;

/* VERTEX */
template <typename T>
class Vertex {
 private:
  T info;
  vector<Edge<T>> adjacency_list;
  bool visited;
  bool processing;
  double cost;
  Vertex* vertex_least_cost;

 public:
  friend class Graph<T>;

  Vertex(T in);
  bool operator<(const Vertex<T>& vertex) const;
  bool operator==(const Vertex<T>& vertex) const;

  void addEdge(Vertex<T>* dest, double weight);
  bool removeEdgeTo(Vertex<T> const* const otherVertex);

  T getInfo() const;
  int getCost() const;
  vector<Edge<T>> getAdjacencyList() const;
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
  for (auto& it_edge : this->adjacency_list) {
    if (it_edge->target == otherVertex) {
      this->adjacency_list.erase(it_edge);
      return true;
    }
  }
  return false;
}

template <typename T>
Vertex<T>::Vertex(T in)
    : info{in},
      visited{false},
      processing{false},
      cost{0},
      vertex_least_cost{NULL} {}

template <typename T>
void Vertex<T>::addEdge(Vertex<T>* target, double weight) {
  Edge<T> new_edge{target, weight};
  this->adjacency_list.push_back(new_edge);
}

template <typename T>
T Vertex<T>::getInfo() const {
  return this->info;
}

template <typename T>
int Vertex<T>::getCost() const {
  return this->cost;
}

template <typename T>
vector<Edge<T>> Vertex<T>::getAdjacencyList() const {
  return this->adjacency_list;
}

template <typename T>
Vertex<T>* Vertex<T>::getPath() const {
  return this->vertex_least_cost;
}

/* EDGE */
template <typename T>
class Edge {
 private:
  Vertex<T>* target;
  double weight;

 public:
  friend class Graph<T>;
  friend class Vertex<T>;

  Edge(Vertex<T>* d, double weight_);
  bool operator<(const Edge<T>& otherEdge) const;

  double getWeight() const;
  Vertex<T>* getTarget() const;
};

template <typename T>
Edge<T>::Edge(Vertex<T>* target_, double weight_)
    : target{target_}, weight{weight_} {}

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
 private:
  set<Vertex<T>*, VertexSetGreaterThan<T>> vertexSet;
  long unsigned int edge_count = 0;
  int numCycles;
  int** W;
  int** P;

  void dfs(Vertex<T>* v, vector<T>& path_history) const;

 public:
  vector<T> dfs() const;
  vector<T> bfs(Vertex<T>* v) const;
  void dijkstra(const T& in);

  bool addVertex(const T& in);
  bool addEdge(const T& in_origin, const T& in_target, double weight);
  bool removeVertex(const T& in);
  bool removeEdge(const T& in_origin, const T& in_target);

  vector<Vertex<T>*> getVertexSet() const;
  long unsigned int getVertexCount() const;
  long unsigned int getEdgeCount() const;
  Vertex<T>* getVertex(const T& in) const;
  vector<Vertex<T>*> getSources() const;
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
vector<Vertex<T>*> Graph<T>::getVertexSet() const {
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
bool Graph<T>::removeVertex(const T& in) {
  auto vertex_in_search = make_shared<Vertex<T>>(in);

  auto it_vertex_info = this->vertexSet.find(vertex_in_search.get());
  if (it_vertex_info == this->vertexSet.end()) {
    return false;
  }

  Vertex<T>* vertex_info = *it_vertex_info;
  vertexSet.erase(it_vertex_info);

  for (const auto& it_vertex : this->vertexSet) {
    (*it_vertex_info)->removeEdgeTo(vertex_info);
  }

  delete vertex_info;
  return true;
}

template <typename T>
bool Graph<T>::addEdge(const T& in_origin, const T& in_target, double weight) {
  auto vertex_origin_search = make_shared<Vertex<T>>(in_origin);
  auto vertex_target_search = make_shared<Vertex<T>>(in_target);

  typename set<Vertex<T>*>::iterator it_vertex_origin =
      this->vertexSet.find(vertex_origin_search.get());
  typename set<Vertex<T>*>::iterator it_vertex_target =
      this->vertexSet.find(vertex_target_search.get());

  auto it_vertex_end = this->vertexSet.end();
  if (it_vertex_origin == it_vertex_end || it_vertex_target == it_vertex_end) {
    return false;
  }

  Vertex<T>* vertex_origin = *it_vertex_origin;
  Vertex<T>* vertex_target = *it_vertex_target;

  vertex_origin->addEdge(vertex_target, weight);

  this->edge_count++;
  return true;
}

template <typename T>
bool Graph<T>::removeEdge(const T& in_origin, const T& in_target) {
  auto vertex_origin_search = make_shared<Vertex<T>>(in_origin);
  auto vertex_target_search = make_shared<Vertex<T>>(in_target);

  auto it_vertex_origin = this->vertexSet.find(vertex_origin_search.get());
  auto it_vertex_target = this->vertexSet.find(vertex_target_search.get());

  auto it_vertex_end = this->vertexSet.end();
  if (it_vertex_origin == it_vertex_end || it_vertex_target == it_vertex_end) {
    return false;
  }

  Vertex<T>* vertex_origin = *it_vertex_origin;
  Vertex<T>* vertex_target = *it_vertex_target;

  if (!vertex_origin->removeEdgeTo(vertex_target)) {
    return false;
  }

  this->edge_count--;
  return true;
}

template <typename T>
vector<T> Graph<T>::dfs() const {
  for (auto it_vertex : this->vertexSet) {
    (*it_vertex)->visited = false;
  }

  vector<T> path_history;
  for (auto it_vertex : this->vertexSet) {
    if (!(*it_vertex)->visited) {
      dfs(*it_vertex, path_history);
    }
  }

  return path_history;
}

template <typename T>
void Graph<T>::dfs(Vertex<T>* vertex, vector<T>& path_history) const {
  vertex->visited = true;
  path_history.push_back(vertex->info);

  for (const auto& it_vertex : vertex->adjacency_list) {
    if (it_vertex->target->visited == false) {
      dfs(it_vertex->target, path_history);
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

    for (auto& it_vertex : selected_vertex->adjacency_list) {
      Vertex<T>* candidate_vertex = it_vertex->target;
      if (!candidate_vertex->visited) {
        candidate_vertex->visited = true;
        fringe.push(candidate_vertex);
      }
    }
  }
  return path_history;
}

template <typename T>
Vertex<T>* Graph<T>::getVertex(const T& in) const {
  auto it_vertex = this->vertexSet.find(&in);
  if (it_vertex == this - vertexSet.end()) {
    return NULL;
  }
  return *it_vertex;
}

template <typename T>
void Graph<T>::dijkstra(const T& in) {
  for (auto& it_vertex : this->vertexSet) {
    (*it_vertex)->vertex_least_cost = NULL;
    (*it_vertex)->cost = std::numeric_limits<double>::max();
    (*it_vertex)->visited = false;
  }

  Vertex<T>* selected_vertex = this->getVertex(in);
  selected_vertex->cost = 0;

  vector<Vertex<T>*> priority_queue;
  priority_queue.push_back(selected_vertex);

  make_heap(priority_queue.begin(), priority_queue.end());

  while (!priority_queue.empty()) {
    selected_vertex = priority_queue.front();
    pop_heap(priority_queue.begin(), priority_queue.end());
    priority_queue.pop_back();

    for (auto& it_edge : selected_vertex->adjacency_list) {
      auto& edge = *it_edge;
      Vertex<T>* target = edge.target;

      double new_target_cost = selected_vertex->cost + edge.weight;
      if (new_target_cost < target->cost) {
        target->cost = new_target_cost;
        target->vertex_least_cost = selected_vertex;

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

#endif /* GRAPH_HPP_ */
