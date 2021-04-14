//
// Created by sergeyampo on 01.04.2021.
//

#ifndef CXXMPI_UNIVERSITY_TASKS_SECOND_SECOND_LIB_INITIALSTATE_H_
#define CXXMPI_UNIVERSITY_TASKS_SECOND_SECOND_LIB_INITIALSTATE_H_

#include <mpi.h>


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

#endif //CXXMPI_UNIVERSITY_TASKS_SECOND_SECOND_LIB_INITIALSTATE_H_
