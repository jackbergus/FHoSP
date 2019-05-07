/*
 * indexing.cpp
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

 
//
// Created by giacomo on 03/11/18.
//
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <map>
#include <vector>

extern "C" {
#include <stdio.h>
}

std::map<unsigned long, unsigned long> indexMapping;
unsigned long index = 0;

unsigned long updatedIndex(unsigned long original) {
    std::map<unsigned long, unsigned long>::iterator lb = indexMapping.lower_bound(original);
    if(lb != indexMapping.end() && !(indexMapping.key_comp()(original, lb->first))) {
        // key already exists
        // update lb->second if you care to
        return lb->second;
    } else {
        // the key does not exist in the map
        // add it to the map
        unsigned long toret = index++;
        indexMapping.insert(lb, std::map<unsigned long, unsigned long>::value_type(original, toret));    // Use lb as a hint to insert,
        return toret;
        // so it can avoid another lookup
    }
}

void clearIndex() {
    indexMapping.clear();
    index = 0;
}


int main(int argc, char** argv) {
    {
        {
            const std::string &edgeFile{"/home/giacomo/fishing_for_fishies/merged/merged_4.txt"};
            std::string edgeFileTMP = edgeFile + "_o.txt";

            {
                std::map<unsigned long, std::vector<std::pair<unsigned long,unsigned long>>> map;
                std::cout << std::endl << edgeFile << std::endl;
                std::ifstream infile(edgeFile);
                unsigned long src, label, dst;
                while (infile >> src >> label >> dst) {
                    src = updatedIndex(src);
                    dst = updatedIndex(dst);
                    map[src].emplace_back(dst, label);
                }

                for (auto it = map.begin(); it != map.end(); it++) {
                    std::sort(it->second.begin(), it->second.end());
                }

                FILE* file = fopen(edgeFileTMP.c_str(), "w");
                for (auto it = map.begin(); it != map.end(); it++) {
                    src = it->first;
                    for (const std::pair<unsigned long,unsigned long>& dst : it->second) {
                        fprintf(file, "%u %u %u\n", src, dst.second, dst.first);
                    }
                }
                fclose(file);
                std::remove(edgeFile.c_str());
                std::rename(edgeFileTMP.c_str(), edgeFile.c_str());
            }

            clearIndex();
        }
    }
}
