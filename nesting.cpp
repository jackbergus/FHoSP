/*
 * nesting.cpp
 * This file is part of FHoSP
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

#include <iostream>
#include <algorithm>
#include <string>
#include <map>
#include <set>
#include <unordered_set>
#include <ctime>
#include "serializers/commons/secondary_memory/header.h"
#include "serializers/commons/secondary_memory/queryresult.h"
#include "serializers/commons/secondary_memory/JOINRESULT.h"
#include "serializers/utils/mmapFile.h"
#include "dependencies/CRoaring/roaring.h"

#include "hashes_paper.h"
#include "serializers/commons/secondary_memory/vertex_id_index.h"

#define FROM_HASH(x)    (((x) & 2) ? KNOWS :(((x) & 4) ? CREATES : (((x) & 8) ? INCLUSTER : 0)))

namespace std {
    template <>
    struct hash<std::pair<LONG_NUMERIC, LONG_NUMERIC>>
    {
        std::size_t operator()(const std::pair<LONG_NUMERIC, LONG_NUMERIC>& k) const
        {
            // Compute individual hash values for first,
            // second and third and combine them using XOR
            // and bit shifting:
            return (((k.first)
                     ^ ((k.second) << 1)) >> 1);
        }
    };

}

int main() {
    std::string base = "/home/giacomo/fishing_for_fishies/merged/shuf_2.txt";
    int fd_values;
    LONG_NUMERIC size_values;
    std::string vertices = base + "_values.bin";
    header* begin_vertices = (header*)mmapFile(vertices.c_str(), &size_values, &fd_values);
    header *v = begin_vertices;
    
    

    std::string vid_idx  = base + "_vid_index.bin";
    std::string cont_idx = base + "_containment.bin";
    std::string adj = base + "_result.bin";
    LONG_NUMERIC size_vid_idx; int fd_vid_idx;
    vertex_id_index* begin_vid_idx = (vertex_id_index*)mmapFile(vid_idx.c_str(), &size_vid_idx, &fd_vid_idx);
    header* end_vertices = (header*)(((char*)begin_vertices) + size_values);
    vertex_id_index* end_vid_idx = (vertex_id_index*)(((char*)begin_vid_idx) + size_vid_idx);
    LONG_NUMERIC V_size = size_vid_idx/sizeof(vertex_id_index);


    std::map<LONG_NUMERIC, std::unordered_set<LONG_NUMERIC>> user_to_cluster;
    std::map<LONG_NUMERIC, std::unordered_set<std::pair<LONG_NUMERIC,LONG_NUMERIC>>> remaining_to_visit_users;
    std::map<LONG_NUMERIC, std::unordered_set<std::pair<LONG_NUMERIC,LONG_NUMERIC>>>::iterator no_remaining = remaining_to_visit_users.end();
    std::map<LONG_NUMERIC, std::unordered_set<std::pair<LONG_NUMERIC,LONG_NUMERIC>>> graph_with_edge_id;

    std::clock_t start = std::clock();
    FILE* containment_index = fopen64(cont_idx.c_str(), "w");
    FILE* adjacencyFile = fopen64(adj.c_str(),"w");

    roaring_bitmap_t *visited_posts = roaring_bitmap_create();
    for (; v < end_vertices; v = vertex_next(v)) {
        if (v->hash == POST) {
            //std::cerr << "DEBUG: vertex " << v->id << " for POST" << std::endl;
            // Setting the current photo as visited
            roaring_bitmap_add(visited_posts, v->id);

            LONG_NUMERIC size;

            // Retreiving all the possible clusters associated to the user
            std::vector<LONG_NUMERIC> clusters;
            struct edges_in_vertices* cluster_edges = vertex_outgoing_edges_vec(v);
            size = vertex_outgoing_edges_len(v);
            for (int i = 0; i<size; i++) {
                clusters.emplace_back(cluster_edges->vertexId);
            }

            // Associating the users to all its clusters
            size = vertex_ingoing_edges_len(v);
            struct edges_in_vertices* users = vertex_ingoing_edges_vec(v);
            for (int i = 0; i<size; i++) {
                for (LONG_NUMERIC j = 0, n = clusters.size(); j<n; j++) {
                    user_to_cluster[users->vertexId].insert(clusters[j]);

                    struct queryresult toWrite;
                    toWrite.id_returned_element = clusters[j];
                    toWrite.containment = queryresylt_opt::IS_VERTEX;
                    toWrite.element = v->id;
                    fwrite(&toWrite, sizeof(struct queryresult), 1, containment_index);
                }
            }
        } else if (v->hash == USER) {
            //std::cerr << "DEBUG: vertex " << v->id << " for USER" << std::endl;
            // Informations concerning the ingoing edges
            LONG_NUMERIC size;
            //const std::map<LONG_NUMERIC, std::unordered_set<std::pair<LONG_NUMERIC,LONG_NUMERIC>>>::iterator &cp = remaining_to_visit_users.find(v->id);

            struct edges_in_vertices* out_edges = vertex_outgoing_edges_vec(v);
            std::vector<std::pair<LONG_NUMERIC , LONG_NUMERIC >> friends;
            size = vertex_outgoing_edges_len(v);
            bool skip = false;

            for (LONG_NUMERIC i = 0; (i<size && (!skip)); i++) {
                unsigned int ll = FROM_HASH(out_edges[i].edgeHash);
                if (ll == CREATES) {
                    if (!roaring_bitmap_contains(visited_posts, out_edges[i].vertexId)) {
                        // If the algorithm hasn't already visited this pic yet, then skip all this process of adding the user and associating that to a cluster
                        skip = true;
                    }
                } else if (ll == KNOWS) {
                    if (!skip) {
                        {
                            LONG_NUMERIC currentFriend = out_edges[i].vertexId;
                            header *f = (header *) (((char *) begin_vertices) + begin_vid_idx[currentFriend].offset);

                            struct edges_in_vertices *f_out_edges = vertex_outgoing_edges_vec(f);
                            LONG_NUMERIC size_f = vertex_outgoing_edges_len(f);
                            bool noSkip = true;
                            for (LONG_NUMERIC j = 0; j < size_f; j++) {
                                if (FROM_HASH(f_out_edges[j].edgeHash) == CREATES) {
                                    if (!roaring_bitmap_contains(visited_posts, f_out_edges[j].vertexId)) {
                                        remaining_to_visit_users[currentFriend].emplace(v->id, f_out_edges[j].edgeId);
                                        noSkip = false;
                                    }
                                }
                            }

                            if (noSkip) for (const LONG_NUMERIC &clI : user_to_cluster[v->id]) {
                                for (const LONG_NUMERIC &clF: user_to_cluster[currentFriend]) {
                                    LONG_NUMERIC edgeId = clI * 1000 + clF + 1;
                                    graph_with_edge_id[clI].emplace(clF, edgeId);
                                    struct queryresult toWrite;
                                    toWrite.id_returned_element = edgeId;
                                    toWrite.containment = queryresylt_opt::IS_EDGE;
                                    toWrite.element = out_edges[i].edgeId;
                                    fwrite(&toWrite, sizeof(struct queryresult), 1, containment_index);
                                    //std::cout << edgeId << " has nesting";
                                }
                            }
                        }

                        /*if (cp != no_remaining) {
                            std::vector<std::pair<LONG_NUMERIC, LONG_NUMERIC >> toRemove{};
                            for (const std::pair<LONG_NUMERIC,LONG_NUMERIC>& friend_ : cp->second) {
                                if (roaring_bitmap_contains(visited_posts, friend_.first)) {
                                    for (const LONG_NUMERIC  &clI : user_to_cluster[v->id]) {
                                        for (const LONG_NUMERIC  &clF: user_to_cluster[friend_.first]) {
                                            LONG_NUMERIC edgeId = clF * 1000 + clI + 1;
                                            graph_with_edge_id[clF].emplace(clI, edgeId);

                                            struct queryresult toWrite;
                                            toWrite.id_returned_element = edgeId;
                                            toWrite.containment = queryresylt_opt::IS_EDGE;
                                            toWrite.element = friend_.second;
                                            fwrite(&toWrite, sizeof(struct queryresult), 1, containment_index);
                                        }
                                    }
                                    toRemove.push_back(friend_);
                                }
                            }
                            for (const std::pair<LONG_NUMERIC, LONG_NUMERIC > & torem : toRemove)
                                cp->second.erase(torem);
                            if (cp->second.empty())
                                remaining_to_visit_users.erase(cp);
                        }*/
                    }
                    friends.emplace_back(out_edges[i].vertexId, out_edges[i].edgeId);
                }
            }

            if (skip) {
                for (const std::pair<LONG_NUMERIC , LONG_NUMERIC > &friend_ : friends) {
                    remaining_to_visit_users[friend_.first].emplace(v->id, friend_.second);
                    /*if (cp != no_remaining) {
                        for (const std::pair<long unsigned int, long unsigned int> &previous : cp->second)
                            remaining_to_visit_users[friend_.first].emplace(previous.first, friend_.second);
                    }*/
                }
                /*if (cp != no_remaining) {
                    remaining_to_visit_users.erase(cp);
                }*/
                continue;
            }


        }
    }

    //std::cout << "REMAINING SIZE: " << remaining_to_visit_users.size() << std::endl;
    for (std::map<LONG_NUMERIC, std::unordered_set<std::pair<LONG_NUMERIC,LONG_NUMERIC>>>::iterator it = remaining_to_visit_users.begin(); it != no_remaining; it++) {
        //std::cout << it->first << std::endl;
        for (const std::pair<LONG_NUMERIC,LONG_NUMERIC>& x: it->second) {
            for (const LONG_NUMERIC &clI : user_to_cluster[x.first]) {
                for (const LONG_NUMERIC &clF: user_to_cluster[it->first]) {
                    LONG_NUMERIC edgeId = clF * 1000 + clI + 1;
                    graph_with_edge_id[clF].emplace(clI, edgeId);

                    struct queryresult toWrite;
                    toWrite.id_returned_element = edgeId;
                    toWrite.containment = queryresylt_opt::IS_EDGE;
                    toWrite.element = x.second;
                    fwrite(&toWrite, sizeof(struct queryresult), 1, containment_index);
                }
            }
        }
    }

    LONG_NUMERIC subgraphs = graph_with_edge_id.size();
    std::map<LONG_NUMERIC, std::unordered_set<std::pair<LONG_NUMERIC,LONG_NUMERIC>>>::iterator it = graph_with_edge_id.begin();
    const std::map<LONG_NUMERIC, std::unordered_set<std::pair<LONG_NUMERIC,LONG_NUMERIC>>>::iterator &end = graph_with_edge_id.end();
    while (it != end) {
        JOINRESULT jr;
        jr.isVertex = 1;
        jr.isEdge = 0;
        jr.left = it->first;
        jr.right = 0;
        fwrite(&jr, sizeof(JOINRESULT), 1, adjacencyFile);
        for (const std::pair<LONG_NUMERIC,LONG_NUMERIC> & is : it->second) {
            subgraphs+=it->second.size();
            JOINRESULT er;
            er.isVertex = 0;
            er.isEdge = 1;
            er.left = is.second;
            er.right = is.first;
            fwrite(&er, sizeof(JOINRESULT), 1, adjacencyFile);
        }
        it++;
    }

    double indexing = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout << indexing << std::endl;

    fclose(adjacencyFile);
    roaring_bitmap_free(visited_posts);
    fclose(containment_index);
    munmap(begin_vertices, size_values);
    munmap(begin_vid_idx, size_vid_idx);
}
