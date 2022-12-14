#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>
#include <unordered_set>
#include <utility>

enum Oper {
    PLUS,
    MULT
};

struct Monkey {
    std::vector<int> items;
    bool square_old;
    Oper oper;
    int oper_num;
};

void day11_1() // unfinished
{
    std::ifstream file("day10.txt");
    if (file.is_open()) {

        std::vector<Monkey> monkeys;

        for (std::string line; std::getline(file, line); )
        {
            if (line.substr(0, 5) == "Monkey") {
                monkeys.push_back({ {}, false, PLUS, 0 });
            }
            else if (line.substr(0, 17) == "  Starting items:") {

            }
        }

        file.close();
    }
}
