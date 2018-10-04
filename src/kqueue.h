//
// Copyright (c) 2018 iTimeTraveler All rights reserved.
//

#ifndef IO_MODELS_STUDY_KQUEUE_H
#define IO_MODELS_STUDY_KQUEUE_H

void updateEvents(int kqfd, int fd, int events, bool is_del);
int kqueue_serv(int argc, char *argv[]);


#endif //IO_MODELS_STUDY_KQUEUE_H
