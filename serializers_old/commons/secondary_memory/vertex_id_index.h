//
// Created by giacomo on 09/11/17.
//
#include "../structure.h"

#ifndef GRAPHSAMPLER_PRIMARY_INDEX_H
#define GRAPHSAMPLER_PRIMARY_INDEX_H

/*
 * Implements the primary index for the GSM structure
 */
struct vertex_id_index {
    vertex_id_index(LONG_NUMERIC id, LONG_NUMERIC hash, LONG_NUMERIC offset) : id(id), hash(hash), offset(offset) {}
    vertex_id_index() : vertex_id_index{0,0,0} {}

    LONG_NUMERIC id;
    LONG_NUMERIC hash;
    LONG_NUMERIC offset;
};

struct hashIndex  {
    LONG_NUMERIC hash;
    LONG_NUMERIC offset;
    hashIndex() {};
    hashIndex(LONG_NUMERIC hash, LONG_NUMERIC offset) :  hash(hash), offset(offset) {}
};


#endif //GRAPHSAMPLER_PRIMARY_INDEX_H
