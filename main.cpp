/*
 * main.cpp
 * This file is part of FHoSP
 *
 * Copyright (C) 2019 - Giacomo Bergami
 *
 * FoSP is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * FoSP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FoSP. If not, see <http://www.gnu.org/licenses/>.
 */


#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <climits>
#include <unordered_set>
#include <set>
#include <ctime>
#include <iostream>
#include <random>
#include <functional>
#include <stack>
#include <queue>

#include "hashes_paper.h"

#include "serializers/commons/Serialize.h"
#include "serializers/commons/primary_memory/entity_relationship.h"
#include "serializers/commons/secondary_memory/vertex_id_index.h"
#include "macros.h"

/**
 * [edgeLabel].emplace_back(dst/src, edgeId)
 * .
 * This map represents an indexed adjacency list for a given vertex, for either the ingoing or the outgoing edges.
 * In particular, each key represents a label associated to at least one edge in the (in/out)going edges. Then, each
 * the value is a set of pairs (vector for efficiency) associating the adjacent vertex (pair::first) and the edge id
 * (pair::second).
 */
#define ADJACENCY_LIST \
std::unordered_map<unsigned short int, std::vector<std::pair<LONG_NUMERIC, LONG_NUMERIC>>>

/**
 * Iterator over the adjacency list for a given vertex.
 */
#define ADJACENCY_LIST_ITERATOR \
ADJACENCY_LIST::iterator


/**
 * [src/dst][edgeLabel].emplace_back(dst/src, edgeId)
 * .
 * This map represents the adjacency list for each given vertex in the graph. It could represent the ingoing edges as ù
 * well as the outgoing edges. See ADJACENCY_LIST for the values associated to the keys (each vertex in the graph)
 */
#define ADJACENCY_MAP \
std::unordered_map<LONG_NUMERIC, ADJACENCY_LIST>

/**
 * Iterator over the adjacency map
 */
#define ADJACENCY_MAP_ITERATOR \
ADJACENCY_MAP::iterator


/**
 *
 * No definition declared: the vertices are serialized in the same order as they appear in the file.
 *
 * WITH_TOPO_ORDERING (tau):  the graph is serialized using the topological ordering
 * WITH_HASH_ORDERING (eta):  the graph is serialized using the vertex label ordering
 *
 * No defined flag means that the induced ordering is the one of the vertices' ids. (iota)
 *
 */

//#define WITH_TOPO_ORDERING
//#define WITH_HASH_ORDERING

#ifdef WITH_HASH_ORDERING
#include <map>
#else
// Providing a specific ordering for the vertices. This could be either the id ordering or the topological sort ordering
std::stack<LONG_NUMERIC> vertexOreder;
#endif


// declaring then topological ordering functions if and only if the topological sorting is setted at compile time
#ifdef WITH_TOPO_ORDERING
// Set of visited vertices that is required by the topological ordering functions
std::unordered_set<LONG_NUMERIC> visited;
POPULATE_TOPO_VISIT_ORDER(populateTopoVisitOrder, std::unordered_set)
POPULATE_TOPO_VISIT_ORDER(populateTopoVisitOrder2, std::set)
#endif


int main() {
    auto gen = std::bind(std::uniform_int_distribution<>(0, 1), std::default_random_engine());
    const unsigned int N = 100;
    std::clock_t start;
    double loading, indexing;
    start = std::clock();

    // Specific implementation of the indexing of the graph
    std::string casus = "default";
#ifdef WITH_TOPO_ORDERING
    casus = "topoOrdering";
#endif
#ifdef WITH_HASH_ORDERING
    casus = "hashing";
#endif

    std::ifstream infile;
    // graph in secondary memory to load
    std::string filename{"/home/giacomo/fishing_for_fishies/merged/shuf_1.txt"};
    LONG_NUMERIC src, dst;
    unsigned short int edgeLabel, srcType, dstType;
    std::set<LONG_NUMERIC> vset1, vset1_cp;
    std::unordered_map<LONG_NUMERIC, LONG_NUMERIC> offsetMap;

    std::unordered_map<LONG_NUMERIC, unsigned short int> vertexLabel;
#ifdef WITH_HASH_ORDERING
    std::map<unsigned short int, std::unordered_set<LONG_NUMERIC>> hashmap;
#endif
    ADJACENCY_MAP ingoingEdges;
    ADJACENCY_MAP adjG1;
#ifdef WITH_TOPO_ORDERING
    std::unordered_set<LONG_NUMERIC> vset2;
    ADJACENCY_MAP adjG2;
#endif
    LONG_NUMERIC startingVertex = ULONG_MAX;

    // Problem: the GMark data format does not provide vertex label information.
    // Therefore, we have to infer that from the edge label

    infile.open(filename);
    LONG_NUMERIC edgeId = 0;
    while (infile >> src >> edgeLabel >> dst) {

        if (edgeLabel == KNOWS) {
            edgeLabel = ((unsigned short int) 2) + (gen() ? ONE : ZERO);
#ifdef WITH_HASH_ORDERING
            hashmap[USER].emplace(src);
            hashmap[USER].emplace(dst);
#endif
            vertexLabel[src] = USER;
            vertexLabel[dst] = USER;
        } else if (edgeLabel == CREATES) {
            edgeLabel = ((unsigned short int) 4) + (gen() ? ONE : ZERO);
#ifdef WITH_HASH_ORDERING
            hashmap[USER].emplace(src);
            hashmap[POST].emplace(dst);
#endif
            vertexLabel[src] = USER;
            vertexLabel[dst] = POST;
        } else if (edgeLabel == INCLUSTER) {
            edgeLabel = ((unsigned short int) 8) + (gen() ? ONE : ZERO);
#ifdef WITH_HASH_ORDERING
            hashmap[POST].emplace(src);
            hashmap[CLUSTERID].emplace(dst);
#endif
            vertexLabel[src] = POST;
            vertexLabel[dst] = CLUSTERID;
        }
        ingoingEdges[dst][edgeLabel].emplace_back(src, edgeId);
#ifndef WITH_HASH_ORDERING
#ifndef WITH_TOPO_ORDERING
        if (vset1.insert(src).second) {
             vertexOreder.push(src);
             vset1_cp.insert(src);
        }
        if (vset1.insert(dst).second) {
             vertexOreder.push(dst);
             vset1_cp.insert(dst);
        }
#endif
#endif

#ifdef WITH_TOPO_ORDERING
        if (src <= dst) {
            startingVertex = (startingVertex <= src) ? startingVertex : src;
            startingVertex = (startingVertex <= dst) ? startingVertex : dst;
#endif
            vset1_cp.insert(src);
            vset1_cp.insert(dst);
            vset1.insert(src);
            vset1.insert(dst);
            adjG1[src][edgeLabel].emplace_back(dst, edgeId);
#ifdef WITH_TOPO_ORDERING
        } else {
            vset2.insert(src);
            vset2.insert(dst);
            adjG2[src][edgeLabel].emplace_back(dst, edgeId);
        }
#endif

        edgeId++;
    }

#ifdef WITH_TOPO_ORDERING
    // Start the topological ordering from the element having more edges, so it will influence less the "wrong ordering"
    // of the graph
    if (vset1.size() > vset2.size()) {
        populateTopoVisitOrder2(adjG1, vset1);
        populateTopoVisitOrder(adjG2, vset2);
    } else {
        populateTopoVisitOrder(adjG2, vset2);
        populateTopoVisitOrder2(adjG1, vset1);
    }
    //vset1.clear();
    vset1 = vset1_cp;
    for (const LONG_NUMERIC &othid : vset2) vset1.insert(othid);
    vset2.clear();
#endif

    LONG_NUMERIC offsetValues = 0;
    indexing = (std::clock() - start) / (double) CLOCKS_PER_SEC;

    std::string adjacency_list_file = filename + "_values.bin";
    FILE *adjacency_list = fopen64(adjacency_list_file.c_str(), "w");

#ifdef WITH_HASH_ORDERING
    std::string hash_primary_index =filename+"_hid_index.bin";
    FILE* hash_index = fopen64(hash_primary_index.c_str(), "w");
#endif

    unsigned short int vertexL;
#ifdef WITH_HASH_ORDERING
    std::map<unsigned short int, std::unordered_set<LONG_NUMERIC>>::iterator it = hashmap.begin(), end = hashmap.end();
    while (it != end) {
        vertexL = it->first;
        struct hashIndex hi{vertexL, offsetValues};
        fwrite(&hi, sizeof(struct hashIndex), 1, hash_index);

        for (const LONG_NUMERIC &vertexId : it->second) {
#else

    while (!vertexOreder.empty()) {
        // Getting the vertex to serialize
        LONG_NUMERIC &vertexId = vertexOreder.top();
        vertexOreder.pop();
        vertexL = vertexLabel[vertexId];
#endif
        // Starting the serialize the ingoing and outgoing element
        offsetMap[vertexId] = offsetValues;
        ERvertex evertex{vertexId, vertexL};
#ifdef WITH_TOPO_ORDERING
        // If I did some topological ordering, some edges might be within this other/additional map
        {
            ADJACENCY_MAP_ITERATOR it = adjG2.find(vertexId);
            if (it != adjG1.cend()) {
                ADJACENCY_LIST_ITERATOR local = it->second.begin(), localEnd = it->second.end();
                while (local != localEnd) {
                    size_t n = local->second.size();
                    if (n > 0) {
                        for (size_t i = 0; i < n; i++) {
                            evertex.outgoingEdges.emplace_back(local->second[i].second, local->first);
                            evertex.outgoingEdges_vertices.emplace_back(local->second[i].first, 0);
                        }
                    }
                    local++;
                }
                it++;
            }
        }
#endif

        {
            // Then, setting either all the outgoing edges or some of them
            ADJACENCY_MAP_ITERATOR it = adjG1.find(vertexId);
            if (it != adjG1.cend()) {
                ADJACENCY_LIST_ITERATOR local = it->second.begin(), localEnd = it->second.end();
                while (local != localEnd) {
                    size_t n = local->second.size();
                    if (n > 0) {
                        for (size_t i = 0; i < n; i++) {
                            evertex.outgoingEdges.emplace_back(local->second[i].second, local->first);
                            evertex.outgoingEdges_vertices.emplace_back(local->second[i].first, 0);
                        }
                    }
                    local++;
                }
                it++;
            }
        }
        {
            // Last, setting the ingoing edges.
            ADJACENCY_MAP_ITERATOR it = ingoingEdges.find(vertexId);
            if (it != ingoingEdges.cend()) {
                ADJACENCY_LIST_ITERATOR local = it->second.begin(), localEnd = it->second.end();
                while (local != localEnd) {
                    size_t n = local->second.size();
                    if (n > 0) {
                        for (size_t i = 0; i < n; i++) {
                            evertex.ingoingEdges.emplace_back(local->second[i].second, local->first);
                            evertex.ingoingEdges_vertices.emplace_back(local->second[i].first, 0);
                        }
                    }
                    local++;
                }
                it++;
            }
        }
        // Writing the values, just as arguments
        offsetValues += serialize(&evertex, adjacency_list);

#ifdef WITH_HASH_ORDERING
        }
        it++;
    }
#else
    }
#endif

    std::string vertex_id_file = filename + "_vid_index.bin";
    FILE *vertex_id_index = fopen64(vertex_id_file.c_str(), "w");
    for (const LONG_NUMERIC &vertexId: vset1) {
        // Writing the primary index for getting the offset where the vertex is stored
        struct vertex_id_index pix{vertexId, vertexLabel[vertexId], offsetMap[vertexId]};
        fwrite(&pix, sizeof(struct vertex_id_index), 1, vertex_id_index);
    }
    fclose(vertex_id_index);

    fclose(adjacency_list);

#ifdef WITH_HASH_ORDERING
    fclose(hash_index);
#endif

    loading = ((std::clock() - start) / (double) CLOCKS_PER_SEC) - indexing;
    std::cout << "Indexing," << casus << "," << indexing << '\n';
    std::cout << "Loading," << casus << "," << loading << '\n';
}
