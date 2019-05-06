//
// Created by giacomo on 15/07/17.
//

#ifndef JINGOLOBA_HEADER_H
#define JINGOLOBA_HEADER_H

extern "C" {
#include <stdint.h>
};

#include <vector>
#include "../../commons/structure.h"

/*typedef struct _containment_ {
    LONG_NUMERIC size;
    LONG_NUMERIC elements[];
} containment;*/

/*
 *
 * TODO: hash cannot be zero. Zero means that any element will do
 */
struct edges_in_vertices {
    LONG_NUMERIC edgeId;
    LONG_NUMERIC edgeHash;
    LONG_NUMERIC vertexId;
    LONG_NUMERIC vertexHash;

    edges_in_vertices() {
        edgeId = edgeHash = vertexHash = vertexId = 0;
    }

    edges_in_vertices(LONG_NUMERIC edgeId, LONG_NUMERIC edgeHash, LONG_NUMERIC vertexId, LONG_NUMERIC vertexHash) {
        this->edgeId = edgeId;
        this->edgeHash = edgeHash;
        this->vertexId = vertexId;
        this->vertexHash = vertexHash;
    }
};

typedef struct _header_ {
    LONG_NUMERIC length;
    LONG_NUMERIC id;
    LONG_NUMERIC hash;
    LONG_NUMERIC edge_containment_offset;
    LONG_NUMERIC attribute_offset;
    LONG_NUMERIC outgoing_offset;
    LONG_NUMERIC ingoing_offset;
    char vertex_containment[];
} header;

#define FOREACH_VERTEX(v)                   for (; v < end; v = vertex_next(v)) {

#define END_VERTEX_FOREACH                  }

/**
 * Obtaining the id of the current vertex
 */
#define vertex_id(ptr)                      (((header*)ptr)->id)

/**
 * Obtaining the hash value associated to the current vertex
 */
#define vertex_hash(ptr)                    (((header*)ptr)->hash)

/**
 * Pointer to the next vertex
 */
#define vertex_next(v)                    ((header*)(((char*)v) + v->length))

/**
 * Number of properties' values associated to the vertex
 */
#define vertex_properties_len(v)          (*((LONG_NUMERIC *) (((char *) v) + v->attribute_offset)))

/**
 * Offset to the i-th value for the current vertex
 */
#define vertex_property_ith_offset(v,i)   (((char *) v) + v->attribute_offset + sizeof(LONG_NUMERIC) * ((i)+1))

/**
 * First property-value's offset for the current vertex
 */
#define vertex_property_ith_begin(v)      (((char *) v) + v->attribute_offset + sizeof(LONG_NUMERIC))

/**
 * Updating the pointer to get the next value
 */
#define vertex_property_ith_next(ptr)       (((char*)ptr) + sizeof(LONG_NUMERIC))

/**
 * Pointer to the i-th property's value
 */
#define vertex_property_ith_value(v,i)    (((char *) v) + v->attribute_offset + *((LONG_NUMERIC *) vertex_property_ith_offset(v,i)) + sizeof(LONG_NUMERIC) * (2))

/**
 * Using the offset to the property's value for getting its associated value
 */
#define vertex_property_ptr_value(v,ptr)    (((char *) v) + v->attribute_offset + *((LONG_NUMERIC *)ptr) + sizeof(LONG_NUMERIC) * (2))

/**
 * Number of outgoing edges for the current vertex
 */
#define vertex_outgoing_edges_len(v)        (*((LONG_NUMERIC*)(((char*)v) + v->outgoing_offset)))

/**
 * Associated vector of the outgoing edges' vector
 */
#define vertex_outgoing_edges_vec(v)        (((struct edges_in_vertices*)(((char*)v) + v->outgoing_offset + sizeof(LONG_NUMERIC))))

/**
 * Number of ingoing edges for the current vertex
 */
#define vertex_ingoing_edges_len(v)         (*((LONG_NUMERIC*)(((char*)v) + v->ingoing_offset)))

/**
 * Associated vector of the ingoing edges' vector
 */
#define vertex_ingoing_edges_vec(v)         (((struct edges_in_vertices*)(((char*)v) + v->ingoing_offset + sizeof(LONG_NUMERIC))))

/**
 * Returns the size of the vertex that the current graph contains
 */
#define vertex_containment_size(ptr)        (*((LONG_NUMERIC *)((ptr)->vertex_containment)))

/**
 * If the size of the vertex containment is not zero, it returns the vector of all the id-sorted vertices
 */
#define vertex_containment(ptr)             (&(((LONG_NUMERIC *)((ptr)->vertex_containment))[1]))


#define edge_containment_size(ptr)          (*(LONG_NUMERIC *)(((char*)ptr) + ptr->edge_containment_offset))


#define edge_containment(ptr)                (&((LONG_NUMERIC *)(((char*)ptr) + ptr->edge_containment_offset))[1])

#endif //JINGOLOBA_HEADER_H
