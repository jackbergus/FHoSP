/*
 * macros.h
 * This file is part of FoSP
 *
 * Copyright (C) 2019 - Giacomo Bergami
 *
 * FoSP is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * FoSP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FoSP. If not, see <http://www.gnu.org/licenses/>.
 */
//
// Created by giacomo on 13/03/19.
//

#ifndef SIMPLIFIED_QOPPA_MACROS_H
#define SIMPLIFIED_QOPPA_MACROS_H

#include <unordered_map>
#include <vector>
#include <set>

//populateTopoVisitOrder

#define POPULATE_TOPO_VISIT_ORDER(name,type)     \
void name (std::unordered_map<LONG_NUMERIC, std::unordered_map<unsigned short int, std::vector<std::pair<LONG_NUMERIC,LONG_NUMERIC>>>>& adjMap, type<LONG_NUMERIC>& toVisit) {\
    std::stack<std::pair<bool, LONG_NUMERIC>> internalStack;\
    std::unordered_map<LONG_NUMERIC, std::unordered_map<unsigned short int, std::vector<std::pair<LONG_NUMERIC,LONG_NUMERIC>>>>::iterator isEnd = adjMap.end();\
    while (!toVisit.empty()) {\
        auto first = toVisit.begin();\
        if (first != toVisit.cend()) {\
            internalStack.push(std::make_pair(false, *first));\
        }\
        while (!internalStack.empty()) {\
            std::pair<bool, LONG_NUMERIC>  cp = internalStack.top();\
            internalStack.pop();\
            toVisit.erase(cp.second);\
            if (cp.first) {\
                vertexOreder.push(cp.second);\
            } else {\
                if (visited.insert(cp.second).second) {\
                    internalStack.push((cp));\
                    internalStack.push(std::make_pair(true, cp.second));\
                    std::unordered_map<LONG_NUMERIC, std::unordered_map<unsigned short int, std::vector<std::pair<LONG_NUMERIC,LONG_NUMERIC>>>>::iterator out = adjMap.find(cp.second);\
                    if (out != isEnd) {\
                        std::unordered_map<unsigned short int, std::vector<std::pair<LONG_NUMERIC,LONG_NUMERIC>>>::iterator it = out->second.begin(), itE = out->second.end();\
                        while (it != itE) {\
                            for (size_t i = 0, n = it->second.size(); i < n; i++) {\
                                internalStack.push(std::make_pair(false, it->second[i].first));\
                            }\
                            it++;\
                        }\
                    }\
                }\
            }\
        }\
    }\
}\

#endif //SIMPLIFIED_QOPPA_MACROS_H
