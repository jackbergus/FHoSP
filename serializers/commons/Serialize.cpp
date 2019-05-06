//
// Created by giacomo on 17/07/17.
//

extern "C" {
#include <stdio.h>
}

#include <iostream>
#include "Serialize.h"
#include "../utils/fwrite.h"

LONG_NUMERIC serialize(ERvertex *v, FILE* file) {

    LONG_NUMERIC size = VERTEX_OFFSET;
    LONG_NUMERIC lastoffset;
    LONG_NUMERIC buffSize = 0;

    header h;
    // starting offset, basis for the vertex containment
    h.length = VERTEX_OFFSET; // TODO!
    // Information that is still there
    h.id = v->id;             // DONE
    h.hash = v->hash;

    /// Evaluating the length
    // 1. vertex containment
    h.length += VECTOR_MEMORY_OCCUPATION(v->vertex_containment);
    h.edge_containment_offset = h.length;
    // 2. edge containment
    h.length += VECTOR_MEMORY_OCCUPATION(v->edge_containment);
    h.attribute_offset = h.length;
    // 3. properties appearing
    h.length += (v->properties.size()+1) * sizeof(LONG_NUMERIC);
    //std::cerr << h.length << std::endl;
    for (struct value &x : v->properties) {
        h.length += ((sizeof(LONG_NUMERIC) * 2)+((x).val.size() + 1)+(sizeof(struct element)*((x).contents.size())));
    }
    h.outgoing_offset = h.length;

    // 4. outgoing outgoingEdges appearing
    h.length += edges_record_entry_size(v->outgoingEdges_vertices);
    h.ingoing_offset = h.length;

    // The length has to consider even the last field, containing the ingoing edges
    h.length += edges_record_entry_size(v->ingoingEdges_vertices);

    my_fwrite(&h, (size_t)sizeof(header), (size_t)1, file);

    /// 1. Serializing the vertex containment
    serializeVector(LONG_NUMERIC, buffSize, v->vertex_containment, file);

    /// 2. Serializing the edge containmnet
    serializeVector(LONG_NUMERIC, buffSize, v->edge_containment, file);

    // -- serializing the size of the properties vector
    buffSize = (v->properties).size();
    my_fwrite(&buffSize, sizeof(buffSize), 1, file);
    if (buffSize > 0) {
        LONG_NUMERIC _fixed_ = (v->properties).size();
        LONG_NUMERIC s = (_fixed_ + 1) * sizeof(LONG_NUMERIC);
        // -- serializing the first element: that is the offset
        my_fwrite(&s, sizeof(LONG_NUMERIC), 1, file);
        LONG_NUMERIC i =  0;
        buffSize = s;
        for (struct value& x : (v->properties)) {
            if (i >= _fixed_-1)
                break;
            else
                i++;
            buffSize +=  ((sizeof(LONG_NUMERIC) * 2)+((x).val.size() + 1)+(sizeof(struct element)*((x).contents.size())));
            // -- Offset for any other element
            my_fwrite(&buffSize, sizeof(buffSize), 1, file);
        }
        // -- For Each Element:
        for (struct value& y : (v->properties)) {
            LONG_NUMERIC element = (y).val.size()+1;
            LONG_NUMERIC contents = (y).contents.size();
            // --- string size
            my_fwrite(&element, sizeof(element), 1, file);
            // --- array size for the mapped contents
            my_fwrite(&contents, sizeof(contents), 1, file);
            // --- serializing the string
            my_fwrite((y).val.c_str(), element, 1, file);
            // --- serializing the mapped contents' vector
            serializeVector2(struct element, y.contents, file);
        }
    }

    lastoffset = v->outgoingEdges_vertices.size();
    my_fwrite(&lastoffset, sizeof(lastoffset), 1, file);
    buffSize = 0;
    for (buffSize = 0; buffSize < lastoffset; buffSize++) {
        my_fwrite(&v->outgoingEdges[buffSize].id, sizeof(v->outgoingEdges[buffSize].id), 1, file);
        my_fwrite(&v->outgoingEdges[buffSize].hash, sizeof(v->outgoingEdges[buffSize].hash), 1, file);
        my_fwrite(&v->outgoingEdges_vertices[buffSize].first, sizeof(v->outgoingEdges_vertices[buffSize].first), 1, file);
        my_fwrite(&v->outgoingEdges_vertices[buffSize].second, sizeof(v->outgoingEdges_vertices[buffSize].second), 1, file);
    }

    lastoffset = v->ingoingEdges_vertices.size();
    my_fwrite(&lastoffset, sizeof(lastoffset), 1, file);
    buffSize = 0;
    for (buffSize = 0; buffSize < lastoffset; buffSize++) {
        my_fwrite(&v->ingoingEdges[buffSize].id, sizeof(v->ingoingEdges[buffSize].id), 1, file);
        my_fwrite(&v->ingoingEdges[buffSize].hash, sizeof(v->ingoingEdges[buffSize].hash), 1, file);
        my_fwrite(&v->ingoingEdges_vertices[buffSize].first, sizeof(v->ingoingEdges_vertices[buffSize].first), 1, file);
        my_fwrite(&v->ingoingEdges_vertices[buffSize].second, sizeof(v->ingoingEdges_vertices[buffSize].second), 1, file);
    }

    return h.length;
}
