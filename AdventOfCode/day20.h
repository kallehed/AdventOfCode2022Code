#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>
#include <unordered_set>
#include <unordered_map>
#include <utility>

inline int modulo(int a, int b) {
    const int result = a % b;
    return result >= 0 ? result : result + b;
}

void day20_1()
{
    std::vector<int> nums;
    {
        std::ifstream file("day20.txt"); // get names of all valves
        if (file.is_open())
        {
            for (std::string line; std::getline(file, line); )
            {
                nums.push_back(std::stoi(line));
            }

            file.close();
        }
    }

    std::vector<int> move = nums;
    std::vector<int> indices;
    for (int i = 0; i < move.size(); ++i) {
        indices.push_back(i);
    }

    for (int i = 0; i < indices.size(); ++i) {
        int index = std::find(indices.begin(), indices.end(), i) - indices.begin();
        std::cout << move[index] << '\n';

        int steps = move[index];

        int start_index = index;
        move.erase(move.begin() + index);
        indices.erase(indices.begin() + start_index);

        index = modulo((steps + index), move.size());

        move.insert(index + move.begin(), steps);
        indices.insert(index + indices.begin(), start_index);
    }

    std::array<int, 3> ways = { 1000,2000,3000 };
    int Total = 0;

    for (int way : ways) {

        int index = std::find(move.begin(), move.end(), 0) - move.begin();

        index = (way + index) % move.size();

        std::cout << "now is: " << move[index];
        Total += move[index];
    }

    std::cout << "TOTAL: " << Total;

}

inline int modulo2(int64_t a, int64_t b) {
    const int64_t result = a % b;
    return result >= 0LL ? result : result + b;
}

void day20_2()
{
    std::vector<int64_t> nums;
    {
        std::ifstream file("day20.txt"); // get names of all valves
        if (file.is_open())
        {
            for (std::string line; std::getline(file, line); )
            {
                int64_t li = std::stoi(line);
                int64_t mid = li * 811589153;
                if ((li > 0 && mid < 0) || (li < 0 && mid > 0)) { // check for overflow
                    std::cout << "ERROR OVERFLOW";
                }
                nums.push_back(mid);
            }

            file.close();
        }
    }

    std::vector<int64_t> move = nums;
    std::vector<int64_t> indices;
    for (int64_t i = 0; i < move.size(); ++i) {
        indices.push_back(i);
    }

    for (int64_t _ = 0; _ < 10; ++_) { // ten times, shuffle it
        for (int64_t i = 0; i < (int64_t)nums.size(); ++i) {
            int64_t index = std::find(indices.begin(), indices.end(), i) - indices.begin();

            int64_t steps = move[index];

            move.erase(move.begin() + index);
            indices.erase(indices.begin() + index);

            index = modulo2((steps + index), move.size());

            move.insert(index + move.begin(), steps);
            indices.insert(index + indices.begin(), i);
        }
    }

    std::array<int64_t, 3> ways = { 1000,2000,3000 };
    int64_t Total = 0;

    for (int64_t way : ways) {

        int64_t index = std::find(move.begin(), move.end(), 0) - move.begin();

        index = (way + index) % move.size();

        std::cout << "now is: " << move[index] << '\n';
        Total += move[index];
    }

    std::cout << "TOTAL: " << Total;

}
