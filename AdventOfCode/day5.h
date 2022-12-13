#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>

void day5_1()
{

    std::array<std::vector<char>, 9> stacks;

    std::ifstream file("day5.txt");
    if (file.is_open()) {
        int line_nbr = 0;
        for (std::string line; std::getline(file, line); )
        {
            if (line_nbr < 8) {
                int place = 1;
                for (int i = 0; i < 9; ++i) {
                    if (line[place] != ' ') {
                        stacks[i].insert(stacks[i].begin(), line[place]);
                    }
                    place += 4;
                }
            }
            else if (line_nbr > 9) {
                size_t first_space = line.find(' ');
                size_t before_second = line.find(' ', first_space + 4);
                size_t before_last = line.find(' ', before_second + 5);
                int how_many = std::stoi(line.substr(first_space + 1));
                int from = std::stoi(line.substr(before_second + 1)) - 1;
                int to = std::stoi(line.substr(before_last + 1)) - 1;

                for (int i = 0; i < how_many; ++i) {
                    char to_move = stacks[from][stacks[from].size() - 1];
                    stacks[to].push_back(to_move);
                    stacks[from].pop_back();
                }
            }
            line_nbr += 1;
        }
        file.close();
    }
    for(auto & e : stacks) {
        std::cout << e[e.size() - 1];
    }
}

void day5_2()
{

    std::array<std::vector<char>, 9> stacks;

    std::ifstream file("day5.txt");
    if (file.is_open()) {
        int line_nbr = 0;
        for (std::string line; std::getline(file, line); )
        {
            if (line_nbr < 8) {
                int place = 1;
                for (int i = 0; i < 9; ++i) {
                    if (line[place] != ' ') {
                        stacks[i].insert(stacks[i].begin(), line[place]);
                    }
                    place += 4;
                }
            }
            else if (line_nbr > 9) {
                size_t first_space = line.find(' ');
                size_t before_second = line.find(' ', first_space + 4);
                size_t before_last = line.find(' ', before_second + 5);
                int how_many = std::stoi(line.substr(first_space + 1));
                int from = std::stoi(line.substr(before_second + 1)) - 1;
                int to = std::stoi(line.substr(before_last + 1)) - 1;

                size_t wher = stacks[from].size() - how_many;

                for (int i = 0; i < how_many; ++i) {
                    char to_move = stacks[from][wher];
                    stacks[to].push_back(to_move);
                    stacks[from].erase(stacks[from].begin() + wher);
                }
            }
            line_nbr += 1;
        }
        file.close();
    }
    for (auto& e : stacks) {
        std::cout << e[e.size() - 1];
    }
}
