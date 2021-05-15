#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <numeric>
#include <iterator>
#include <algorithm>
#include "mpi.h"
#include "lib/MPI_State.h"


using namespace std;

void star(const vector<string>& msgs_broadcast, MPI_State& state) {
  const auto str_to_send = accumulate(begin(msgs_broadcast), end(msgs_broadcast), string(""));
  auto message_sz = msgs_broadcast[0].size();
  auto received_buffer = new char[message_sz];

  MPI_Scatter(str_to_send.c_str() - message_sz,
			  message_sz,
			  MPI_CHAR,
			  received_buffer,
			  message_sz,
			  MPI_CHAR,
			  0,
			  MPI_COMM_WORLD);
  if (state.proc_rank == 0) {
	cout << "Parent process sent: " << str_to_send << " separate it by chunks with size: " << message_sz
		 << "\n";
  } else {
	cout << "Received from parent process: ";
	copy(received_buffer, received_buffer + message_sz, ostream_iterator<char>(cout, ""));
	cout << "\n";
  }

  auto gathered_buf = new char[str_to_send.size()];
  //Gather
  MPI_Gather(received_buffer, message_sz, MPI_CHAR, gathered_buf, message_sz, MPI_CHAR, 0, MPI_COMM_WORLD);
  if (state.proc_rank == 0) {
	cout << "Parent process gathered message: ";
	copy(gathered_buf + message_sz, gathered_buf + message_sz + str_to_send.size(), ostream_iterator<char>(cout, ""));
	cout << "\n";
  } else {
	cout << "Process #" << state.proc_rank << " sent the message: ";
	copy(received_buffer, received_buffer + message_sz, ostream_iterator<char>(cout, ""));
	cout << "\n";
  }
}

bool is_vector_correct(const vector<string>& vec, const MPI_State& state) {
  return all_of(begin(vec),
				end(vec),
				[&](const string& el) {
				  return vec[0].size() == el.size() &&
					  vec.size() == state.proc_num-1;
				});
}

int main(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);
  const int M = 1;
  auto state = initial_state();
  //Each word should be equal size
  vector<string> vec = {"zero|", "firs|", "seco|", "thir|"};
  if (!is_vector_correct(vec, state)) {
	cout << "Input data is incorrect!" << "\n";
	return 1;
  }
  for(int i = 0; i < M; ++i) {
	star(vec, state);
  }

  MPI_Finalize();

  return 0;
}
