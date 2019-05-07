//
// Created by giacomo on 09/11/17.
//

#include "element.h"
#include <string>
#include <vector>

#ifndef GRAPHSAMPLER_VALUE_H
#define GRAPHSAMPLER_VALUE_H

/**! Properties associated to each vertex and edge
 */
struct value {
    std::string property; /**< describing the attribute in some human readdable format.
                           *   I assume that each field within the struct value is sorted by its property value,
                           *   and that all the properties belonging to the whole graph are represented. When a
                           *   vertex misses that specific value, the value is set to an empty string
                           **/

    std::string val;    /**< value description. It could be either a constant or an expression (TODO) */
    std::vector<struct element> contents; /**< I assume that they are already sorted by type, and then by id */

    value(std::string prop, std::string val) : property{prop}, val{val} {};
    value() : value("","") {};
};

#endif //GRAPHSAMPLER_VALUE_H
