//
// Created by giacomo on 11/11/17.
//

#ifndef GRAPHSAMPLER_GRAPHCOLLECTIONMAPPER_H
#define GRAPHSAMPLER_GRAPHCOLLECTIONMAPPER_H

#include <string>
#include <boost/filesystem/path.hpp>
#include <iostream>
#include "../../../utils/mmapFile.h"
#include "../vertex_id_index.h"
#include "../header.h"

class GraphCollectionMapper {
    unsigned long primary_index_size;
    int primary_index_fd;
    unsigned long containment_size;
    int containment_fd;

public:
    struct primary_index* index = nullptr;
    header* containment = nullptr;

    GraphCollectionMapper(char* operandPath);
    ~GraphCollectionMapper();

    LONG_NUMERIC vertexSetSize();
};

/**
 * returns all the graphs stored in the current graph collection as vertices
 */
#define gc_vertex_containment(gc)           vertex_containment((gc).containment)

/**
 * returns the size of all the graphs stored in the current graph collection as vertices
 */
#define gc_vertex_containment_size(gc)      vertex_containment_size((gc).containment)

// helper macro returning the header associated to the current Graph, starting from id=1
#define _gc_get_graph_header(gc,i)          ((header*)(((char*)(gc).containment)+(gc).index[i].offset))

#define g_graph_id(gc,i)                    vertex_id(_gc_get_graph_header(gc,i))

/**
 * If the size of the vertex containment is not zero, it returns the vector of all the id-sorted vertices contained in
 * the current graph
 *
 * i = 1-based count
 */
#define g_vertex_containment(gc,i)          vertex_containment(_gc_get_graph_header(gc,i))

/**
 * Returns the size of the vertex that the current graph contains
 *
 * i = 1-based count
 */
#define g_vertex_containment_size(gc,i)     vertex_containment_size(_gc_get_graph_header(gc,i))

#define g_edge_containment(gc,i)            edge_containment(_gc_get_graph_header(gc,i))

#define g_edge_containment_size(gc,i)       edge_containment_size(_gc_get_graph_header(gc,i))

#define FOREACH_GRAPH_IN_COLLECTION(var,vCont,nCollections,gc) {\
        LONG_NUMERIC *vCont = gc_vertex_containment(gc);\
        LONG_NUMERIC nCollections = gc_vertex_containment_size(gc);\
        for (unsigned long var = 1; var<=nCollections; var++) {

#define END_FOREACH_GRAPH                 }}

#endif //GRAPHSAMPLER_GRAPHCOLLECTIONMAPPER_H

