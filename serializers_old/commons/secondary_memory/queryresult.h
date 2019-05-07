//
// Created by giacomo on 02/08/17.
//

#ifndef PROJECT_QUERYRESULT_H
#define PROJECT_QUERYRESULT_H

#include "../structure.h"

enum queryresylt_opt {
    IS_VERTEX = 0,
    IS_EDGE = 1,
    IS_EDGE_SRC = 4,
    IS_EDGE_DST = 5,
    IS_VERTEX_CONTAINMENT = 2,
    IS_EDGE_CONTAINMENT = 3
};

struct queryresult {
    LONG_NUMERIC id_returned_element;
    enum queryresylt_opt containment;
    LONG_NUMERIC element;
};

#endif //PROJECT_QUERYRESULT_H
