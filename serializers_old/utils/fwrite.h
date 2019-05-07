//
// Created by giacomo on 18/07/17.
//

#ifndef PROJECT_FWRITE_H
#define PROJECT_FWRITE_H

#include <stdio.h>
#include <string.h>

#ifndef DEBUG
#define my_fwrite   fwrite
#else
size_t my_fwrite(const void *src, size_t size, size_t nmemb, void *file);
#endif

#endif //PROJECT_FWRITE_H
