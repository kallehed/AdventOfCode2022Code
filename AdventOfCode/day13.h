#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>
#include <unordered_set>
#include <utility>

struct List {
    List* next = nullptr;
    int num = -1;
};

void day12_1()
{


    std::ifstream file("day13.txt");
    if (file.is_open())
    {
        List twos[2];
        int mod = 0;
        for (std::string line; std::getline(file, line); )
        {

            List& cur = twos[mod];

            ++mod;
            if (line.length() == 0) {


                mod = 0;
            }
        }
        file.close();
    }

}
