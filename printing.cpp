/*
 * printing.cpp
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
// Created by giacomo on 14/03/19.
//

#include <iostream>
#include <fstream>
#include <map>

int main() {

    std::ifstream file1, file2, file3, file4;
    file1.open("/media/giacomo/Data/DatasetJournal/gmark/demo/social/social-graph.txt0.txt");
    file2.open("/media/giacomo/Data/DatasetJournal/gmark/demo/social/social-graph.txt1.txt");
    file3.open("/media/giacomo/Data/DatasetJournal/gmark/demo/social/social-graph.txt2.txt");
    file4.open("/media/giacomo/Data/DatasetJournal/gmark/demo/social/social-graph.txt3.txt");

    std::map<unsigned long int, unsigned long int> map1, map2, map3, map4;
    unsigned long int count = 0;

    std::cout << "map1" << std::endl;
    unsigned long int src, label, dst;
    while (file1 >> src >> label >> dst) {
        if (label == 2) {
            if (map1.find(dst) == map1.end()) {
                map1[dst] = count++;
                std::cout << dst << "=" << count-1 << std::endl;
            }
        }
    }

    count = 0;
    std::cout << "map2" << std::endl;
    while (file2 >> src >> label >> dst) {
        if (label == 2) {
            if (map2.find(dst) == map2.end()) {
                map2[dst] = count++;
                std::cout << dst << "=" << count-1 << std::endl;
            }
        }
    }


    count = 0;
    std::cout << "map3" << std::endl;
    while (file3 >> src >> label >> dst) {
        if (label == 2) {
            if (map3.find(dst) == map3.end()) {
                map3[dst] = count++;
                std::cout << dst << "=" << count-1 << std::endl;
            }
        }
    }


    count = 0;
    std::cout << "map4" << std::endl;
    while (file4 >> src >> label >> dst) {
        if (label == 2) {
            if (map4.find(dst) == map4.end()) {
                map4[dst] = count++;
                std::cout << dst << "=" << count-1 << std::endl;
            }
        }
    }
}
