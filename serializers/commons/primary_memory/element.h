//
// Created by giacomo on 09/11/17.
//

#ifndef GRAPHSAMPLER_ELEMENT_H
#define GRAPHSAMPLER_ELEMENT_H

#include "../structure.h"

/**! An element describing a list of elements associated to a value
 */
struct element {
    LONG_NUMERIC value_type;
    LONG_NUMERIC id;
    element(LONG_NUMERIC f, LONG_NUMERIC s) : value_type{f}, id{s} {};
};


#endif //GRAPHSAMPLER_ELEMENT_H
