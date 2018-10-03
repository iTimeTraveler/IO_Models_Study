//
// Copyright (c) 2018 iTimeTraveler All rights reserved.
//

#ifndef IO_MODELS_STUDY_POLL_SERVER_H
#define IO_MODELS_STUDY_POLL_SERVER_H

#include <poll.h>

void print_pollsets(std::vector<pollfd> poll_sets);
int poll_serv(int argc, char *argv[]);

#endif //IO_MODELS_STUDY_POLL_SERVER_H
