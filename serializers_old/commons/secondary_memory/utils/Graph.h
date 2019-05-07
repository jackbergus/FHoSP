//
// Created by giacomo on 05/11/17.
//

#ifndef NESTING_GRAPH_H
#define NESTING_GRAPH_H

#include <set>
#include <dirent.h>
#include <boost/filesystem/path.hpp>
#include "../header.h"
#include "../../../utils/mmapFile.h"
#include "../vertex_id_index.h"

class Graph {
    unsigned long size = 0;
    unsigned long sizei = 0;
    int fd = 0;
    int fdi = 0;
    std::string handle;

public:
    header* begin = nullptr;
    header* end = nullptr;
    struct primary_index* index = nullptr;

    Graph(char* path);

    unsigned long nVertices();

    bool open();

    bool clos();

    ~Graph();
};


#endif //NESTING_GRAPH_H
