//
// Created by sergeyampo on 08.05.2021.
//

#ifndef CXXMPI_UNIVERSITY_TASKS_SECOND_FOURTH_BINARYROUTES_H_
#define CXXMPI_UNIVERSITY_TASKS_SECOND_FOURTH_BINARYROUTES_H_

#include <vector>
#include <stdexcept>
#include <queue>
#include <iterator>
#include <algorithm>
#include <numeric>

struct Route {
  uint from;
  uint to;
};

class BinaryRoutes {
 private:
  std::priority_queue<Route,
					  std::vector<Route>,
					  std::function<bool(const Route&, const Route&)>> routes;

  //ptr - указатель на начало массива, l - длина массива,
  //i - уровень дерева, j - индекс вершины
  void g(std::vector<uint>::const_iterator beg, uint l) {
	l /= 2; // l всегда четно
	routes.push({beg[l], *beg});
	if (l == 1) {
	  l *= 2;

	  return;
	}
	g(beg, l);
	g(beg + l, l);
  }

 public:
  explicit BinaryRoutes(const uint& prc_amt) {
	routes = decltype(routes)(([](const Route& lhs, const Route& rhs) -> bool {
	  return (lhs.from - lhs.to) > (rhs.from - rhs.to);
	}));
	std::vector<uint> vec(prc_amt);
	std::iota(vec.begin(), vec.end(), 0);
	g(vec.begin(), prc_amt);
  }

  void pop() {
	routes.pop();
  }

  template<typename T>
  void print(T q) {
	while (!q.empty()) {
	  std::cout << "(" << q.top().from << ";" << q.top().to << ")" << ' ';
	  q.pop();
	}
	std::cout << std::endl;
  }

  Route top() {
	return routes.top();
  }

  bool empty() {
	return routes.empty();
  }
};

#endif //CXXMPI_UNIVERSITY_TASKS_SECOND_FOURTH_BINARYROUTES_H_
