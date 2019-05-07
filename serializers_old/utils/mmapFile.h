//
// Created by giacomo on 04/08/17.
//

#ifndef PROJECT_MMAPFILE_H
#define PROJECT_MMAPFILE_H

#include <string>
#include <fstream>

extern "C" {
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
}

void* mmapFile(std::string file, unsigned long* size, int* fd);

#endif //PROJECT_MMAPFILE_H
