//
// Copyright (c) 2018 iTimeTraveler All rights reserved.
//

#ifndef IO_MODELS_STUDY_SELECT_SERVER_H
#define IO_MODELS_STUDY_SELECT_SERVER_H

#include <set>

void print_fdsets(std::set<int> fd_sets, fd_set readfds);
int select_serv(int argc, char *argv[]);

#endif //IO_MODELS_STUDY_SELECT_SERVER_H
