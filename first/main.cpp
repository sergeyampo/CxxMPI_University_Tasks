#include <iostream>
#include "mpi.h"


using namespace std;

struct MPI_State {
  int proc_num;
  int proc_rank;
  MPI_Status status;
};

MPI_State initial_state() {
  MPI_State state;
  MPI_Comm_size(MPI_COMM_WORLD, &state.proc_num);
  MPI_Comm_rank(MPI_COMM_WORLD, &state.proc_rank);

  return state;
}

//Send to parent process
void star(const int* msg, const int& size, MPI_State& state) {
	cout << "#" << state.proc_rank << " is sending " << *msg << " to the parent!" << "\n";
	auto result = MPI_Send(msg, size, MPI_INT, 0, 0, MPI_COMM_WORLD);
	if (result != MPI_SUCCESS) {
	  throw new std::runtime_error("Unable to send a message to start a chain of messages!");
	}
}

void listen_and_respond(const int* respond_msg, const int& count, MPI_State& state){
  int input_message;
  MPI_Bcast(&input_message, count, MPI_INT, 0, MPI_COMM_WORLD);
  cout << "Got " << input_message << " message from parent by process #" << state.proc_rank << "\n";

  cout << "#" << state.proc_rank << " is sending " << *respond_msg << " to the parent again!" << "\n";
  auto result = MPI_Send(respond_msg, count, MPI_INT, 0, 0, MPI_COMM_WORLD);
  if (result != MPI_SUCCESS) {
	throw new std::runtime_error("Unable to send a message to start a chain of messages!");
  }
}

int main(int argc, char* argv[]) {
  const int message = 12;
  int message_sec = 24;
  const int sz = 1;
  unsigned short M = 2;

  MPI_Init(&argc, &argv);
  while(M-- > 0) {

	auto state = initial_state();
	if (state.proc_rank == 0) {
	  int input_message;
	  MPI_Recv(&input_message, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &state.status);
	  cout << "The message was recieved " << "and it's: " << input_message << "\n";
	  MPI_Bcast(&message_sec, sz, MPI_INT, 0, MPI_COMM_WORLD);
	  cout << "Send everyone " << message_sec << endl;

	}
	  //Child processes listens and responds to the main
	else {
	  star(&message, sz, state);
	  listen_and_respond(&message_sec, sz, state);
	}
  }

  MPI_Finalize();

  return 0;
}
