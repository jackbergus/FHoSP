//
// Created by giacomo on 17/07/17.
//

#ifndef JINGOLOBA_SERIALIZE_H
#define JINGOLOBA_SERIALIZE_H


#include "primary_memory/entity_relationship.h"
#include "secondary_memory/header.h"



/*
 * TODO: uniform code using iterators
 *
struct pointerForEdges {
    vertex*       ptr;
    LONG_NUMERIC  count;
    LONG_NUMERIC* id;
    LONG_NUMERIC* hash;
    LONG_NUMERIC* out_vertex;

    bool operator==(const pointerForEdges& right);
    bool operator!=(const pointerForEdges& right);
};

class edgeIteratorForVertices : public iterator<struct pointerForEdges> {
    struct pointerForEdges* external;
    LONG_NUMERIC begin_int = 0;
    LONG_NUMERIC size = 0;

public:
    edgeIteratorForVertices(struct pointerForEdges* external);
    struct pointerForEdges* next();
    struct pointerForEdges* peek();
    bool hasNext();

    bool operator==(const edgeIteratorForVertices& right);
    bool operator!=(const edgeIteratorForVertices& right);
    LONG_NUMERIC begin() {
        return begin_int;
    }
    LONG_NUMERIC end() {
        return (external != nullptr) ? external->ptr->outgoingEdges_vertices.size();
    }
};
*/

LONG_NUMERIC serialize(ERvertex* v, FILE* file);


#endif //JINGOLOBA_SERIALIZE_H
