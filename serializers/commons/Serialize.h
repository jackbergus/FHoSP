//
// Created by giacomo on 17/07/17.
//

#ifndef JINGOLOBA_SERIALIZE_H
#define JINGOLOBA_SERIALIZE_H


#include "primary_memory/entity_relationship.h"
#include "secondary_memory/header.h"

#define serializeVector(type,sizeVariable,vectorVariable,file) {\
                        sizeVariable = (vectorVariable).size();\
                        my_fwrite(&sizeVariable, sizeof(sizeVariable), 1, file);\
                        for (type x : (vectorVariable)) {\
                            my_fwrite(&x, sizeof(type), 1, file); \
                        }\
}

#define serializeVector2(type,vectorVariable,file) {\
                        for (type x : (vectorVariable)) {\
                            my_fwrite(&x, sizeof(type), 1, file); \
                        }\
}

/**! Serializes the vector contaning mutable representations
 * @arg type:               type of the elements of the vector
 * @arg sizeVariable:       variable where to store the final size of the record
 * @arg vectorVariable:     vector containing all the elements
 * @arg file:               Where to write the serialization
 * @arg recordserializer:   Serializes each element of the field
 * @arg recordSizeExtimator:Evaluates the serialized size of the stuff
 */
#define serializeMutableVector(type, sizeVariable, vectorVariable, file, recordserializer, recordSizeExtimator) {\
                        sizeVariable = (vectorVariable).size();\
                        my_fwrite(&sizeVariable, sizeof(sizeVariable), 1, file);\
                        if (sizeVariable > 0) {\
                            LONG_NUMERIC _fixed_ = (vectorVariable).size();\
                            LONG_NUMERIC s = (_fixed_ + 1) * sizeof(LONG_NUMERIC);\
                            my_fwrite(&s, sizeof(LONG_NUMERIC), 1, file);\
                            /*for (int _i_ = 0; (_i_+1) < sizeVariable; _i_ ++ ) {\
                                sizeVariable += recordSizeExtimator(vectorVariable[_i_]);\
                                my_fwrite(&sizeVariable, sizeof(sizeVariable), 1, file);\
                            }*/\
                            s = 0;\
                            for (type x : (vectorVariable)) {\
                                if (s >= _fixed_-1) break; else s++;\
                                std::cout << s<< _fixed_ -1 << std::endl; sizeVariable += recordSizeExtimator(x);\
                                my_fwrite(&sizeVariable, sizeof(sizeVariable), 1, file);\
                            }\
                            for (type x : (vectorVariable)) {\
                                recordserializer(x, file); \
                            }\
                        }\
}

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
