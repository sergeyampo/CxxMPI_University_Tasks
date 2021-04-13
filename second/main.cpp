//
// Created by sergeyampo on 01.04.2021.
//
#include <iostream>
#include <random>
#include <climits>
#include "lib/MPI_State.h"

using namespace std;

static std::random_device rd;
static std::mt19937 rng{rd()};

unsigned long long dice(const unsigned long long& a, const unsigned long long& b){
  static std::uniform_int_distribution<unsigned long long> uid(a,b);
  return uid(rng);
}

int main(int argc, char* argv[]){
  MPI_Init(&argc, &argv);
  int procAmount, iterations;
  procAmount = atoi(argv[1]);
  iterations = atoi(argv[2]);
  auto state = initial_state();

  unsigned long long sum = 0;
  unsigned long long randomNumber = dice(0, 5000);

  for(int i = 0; i < iterations; ++i){
    if(state.proc_rank == 0) {
	  if (sum == 0) {
		MPI_Bcast(&randomNumber, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);
		cout << "Parent first time sent " << randomNumber << " to everyone!\n";
	  } else {
		sum += 1;
		MPI_Bcast(&sum, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);
		cout << "Parent " << iterations << " time sent " << randomNumber << " to everyone!\n";
	  }
	}
	  unsigned long long recieved_data = 0;
	  if(state.proc_rank != 0) {
		MPI_Bcast(&recieved_data, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);
		cout << "#" << state.proc_rank << " recieved: " << recieved_data << "\n";
		recieved_data += state.proc_rank;
	  }

	if(state.proc_rank == 0){
	  cout << "recieved_data " << recieved_data << "\n";
	}
    MPI_Reduce(&recieved_data, &sum, 1, MPI_UNSIGNED_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
	if(state.proc_rank != 0){
	  cout << "#" << state.proc_rank << " send with reduce: " << recieved_data << "\n";
	}
	else{
	  cout << "Parent reduced all values and get: " << sum << "\n";
	}
  }

  MPI_Finalize();
  return 0;
}

