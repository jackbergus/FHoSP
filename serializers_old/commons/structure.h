//
// Created by giacomo on 17/07/17.
//

#ifndef JINGOLOBA_STRUCTURE_H
#define JINGOLOBA_STRUCTURE_H

//#define PRINTDEBUG

#include <stdint-gcc.h>

typedef uint_fast64_t
        LONG_NUMERIC;

typedef uint_fast32_t
        UINT;

#define NUMERIC_MEMORY_OCCUPATION  \
        (sizeof(LONG_NUMERIC))

#include <vector>
typedef std::vector<LONG_NUMERIC>
        NUMERIC_VECTOR;

typedef std::vector<std::pair<LONG_NUMERIC, LONG_NUMERIC>>
        PAIR_NUMERIC_VECTOR;

#define VECTOR_SIZE(v) \
        ((v).size())

#define VECTOR_MEMORY_OCCUPATION(v)\
        (((VECTOR_SIZE(v))+1)*(NUMERIC_MEMORY_OCCUPATION))

#define VERTEX_OFFSET \
        (sizeof(header))

#include "secondary_memory/header.h"

/*bool binsearch_edges_in_vertices(struct edges_in_vertices* e, LONG_NUMERIC h_e, LONG_NUMERIC h_v, LONG_NUMERIC size,
                                 LONG_NUMERIC* left_limit, LONG_NUMERIC* right_limit);*/

#define EDGE_ID     0
#define EDGE_HASH   1
#define VERTEX_ID   2
#define VERTEX_HASH 3

#define get_field_from(part,casus, val) switch (casus) {\
        case EDGE_ID:  val = part.edgeId; break;\
        case EDGE_HASH:  val = part.edgeHash; break;\
        case VERTEX_ID:  val = part.vertexId; break;\
        default: val = part.vertexHash; break;\
    }


#endif //JINGOLOBA_STRUCTURE_H
