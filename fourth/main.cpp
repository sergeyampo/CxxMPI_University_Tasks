#include "mpi.h"
#include "lib/MPI_State.h"
#include <iostream>
#include <random>
#include <functional>
#include <iterator>
#include "BinaryRoutes.h"


const unsigned number_chunks = 20;
using namespace std;

class LongNumber {
 private:
  vector<uint64_t> data;

 public:
  LongNumber(auto generator, const vector<uint64_t>::size_type& sz) {
	data = vector<uint64_t>(sz);
	generate(begin(data), end(data), generator);
  }

  explicit LongNumber(const vector<uint64_t>& rawData) {
	data = rawData;
  }

  friend ostream& operator<<(ostream&, const LongNumber&);

  vector<uint64_t>::size_type size() const noexcept(true) {
	return data.size();
  }

  LongNumber operator*(const LongNumber& rhs) {
	vector<uint64_t> result_num(data.size() + rhs.data.size(), 0);
	for (uint64_t ix = 0; ix < data.size(); ix++) {
	  for (uint64_t jx = 0; jx < rhs.data.size(); jx++) {
		result_num[ix + jx] += data[ix] * rhs.data[jx];
	  }
	}

	for (uint64_t ix = 0; ix < data.size() + rhs.data.size(); ix++) {
	  result_num[ix + 1] += result_num[ix] / 10;
	  result_num[ix] %= 10;
	}

	return LongNumber{result_num};
  }

  //Вставляем размер на первую позицию
  uint64_t* get_data() {
    data.insert(begin(data), 1, data.size());
	return data.data();
  }
};

ostream& operator<<(ostream& out, const LongNumber& v) {
  bool is_first_num = false;
  copy_if(rbegin(v.data), rend(v.data), ostream_iterator<uint64_t>(out), [&](const uint64_t& e) {
	//Вырезаем первый ноль при обратном чтении
	bool cmp = e != 0 || is_first_num || e <= 9;
	is_first_num = true;
	return cmp;
  });
  return out;
}

//Инициализироваться
//Перемножить два буффера
int main(int argc, char* argv[]) {
  random_device rnd_device;
  mt19937 mersenne_engine{rnd_device()};
  uniform_int_distribution<int> dist{1, 9};
  auto gen = [&dist, &mersenne_engine]() { return dist(mersenne_engine); };

  MPI_Init(&argc, &argv);
  auto state = initial_state();

  cout << "Process #" << state.proc_rank << ": ";
  LongNumber num_f(gen, number_chunks);
  LongNumber num_s(gen, number_chunks);
  auto num_res = num_f * num_s;
  cout << num_f << " * " << num_s << " = " << num_res << endl;

  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Datatype num;
  MPI_Type_contiguous(number_chunks * 2 * state.proc_num + 1, MPI_UNSIGNED_LONG_LONG, &num); //+1 под размер
  MPI_Type_commit(&num);

  BinaryRoutes routes(16);
  routes.print(routes);

  //(1;0) (5;4) (7;6) (3;2) (2;0) (6;4) (4;0)

  uint curr_route_difference = 1;
  while (!routes.empty()) {
	do {
	  Route route = routes.top(); //(1;0)
	  if(curr_route_difference != (route.from - route.to)){
	    break;
	  }
	  routes.pop(); //(2;0)
	  if (route.from == state.proc_rank) {
		MPI_Send(num_res.get_data(), 1, num, route.to, 1, MPI_COMM_WORLD);
		cout << "Proc #" << state.proc_rank << " sent " << num_res << " to " << route.to << endl;
	  } else if (route.to == state.proc_rank) {
	    uint64_t* received_buf = new uint64_t[number_chunks * 2 * state.proc_num + 1];
		MPI_Recv(received_buf, 1, num, route.from, MPI_ANY_TAG, MPI_COMM_WORLD, &state.status);
		LongNumber received_long_num(vector<uint64_t>(received_buf, received_buf + number_chunks * 2 * state.proc_num + 1));
		cout << "Proc #" << state.proc_rank << " received " << received_long_num << " to " << route.to << endl;
	  }
	  ++curr_route_difference;
	} while (true);
	MPI_Barrier(MPI_COMM_WORLD);
  }

  return 0;
}
