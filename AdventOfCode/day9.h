#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>
#include <unordered_set>
#include <utility>

bool operator == (std::pair<int,int> const& lhs, std::pair<int, int> const& rhs)
{
    return lhs.first == rhs.first && lhs.second == rhs.second;
}

struct Hash {
    size_t operator()(const std::pair<int,int>& nums) const {
        return (size_t)(std::abs(nums.first) * std::abs(nums.second));
    }
};

void day9_1()
{
    std::array<std::pair<char, int>, 2000> actions = { { { ' ', 0 } } };

    std::ifstream file("day9.txt");
    if (file.is_open()) {
        int i = 0;
        for (std::string line; std::getline(file, line); )
        {
            actions[i].first = line[0];
            actions[i].second = std::stoi(line.substr(2));
            ++i;
        }
        file.close();
    }    

    int head_x = 0;
    int head_y = 0;
    int tail_x = 0;
    int tail_y = 0;
    std::unordered_set<std::pair<int, int>, Hash> poses;

    for (auto& ac : actions) {
        for (int _ = 0; _ < ac.second; _++) {
            if      (ac.first == 'U') { head_y++; }
            else if (ac.first == 'D') { head_y--; }
            else if (ac.first == 'R') { head_x++; }
            else if (ac.first == 'L') { head_x--; }
            else { std::cout << "ERROR"; std::cin.get(); }

            int dx = head_x - tail_x;
            int dy = head_y - tail_y;

            if (dx == 2 && dy == 1) {
                ++tail_x; ++tail_y;
            }
            if (dx == 2 && dy == -1) {
                ++tail_x; --tail_y;
            }
            if (dx == 1 && dy == 2) {
                ++tail_x; ++tail_y;
            }
            if (dx == 1 && dy == -2) {
                ++tail_x; --tail_y;
            }
            if (dx == -1 && dy == 2) {
                --tail_x; ++tail_y;
            }
            if (dx == -1 && dy == -2) {
                --tail_x; --tail_y;
            }
            if (dx == -2 && dy == 1) {
                --tail_x; ++tail_y;
            }
            if (dx == -2 && dy == -1) {
                --tail_x; --tail_y;
            }
            if (dx == -2 && dy == 0) { --tail_x; }
            if (dx == 2 && dy == 0) { ++tail_x; }
            if (dy == -2 && dx == 0) { --tail_y; }
            if (dy == 2 && dx == 0) { ++tail_y; }

            poses.insert({ tail_x, tail_y });

            if (std::abs(tail_x - head_x) > 1 || std::abs(tail_y - head_y) > 1) { std::cout << "ER " << tail_x << ' ' << tail_y << ' ' << head_x << ' ' << head_y << ' ' << ac.first; std::cin.get(); }
        }
    }

    int count = 0;
    count = (int)poses.size();
    std::cout << count;
}

void day9_2()
{
    std::array<std::pair<char, int>, 2000> actions = { { { ' ', 0 } } };

    std::ifstream file("day9.txt");
    if (file.is_open()) {
        int i = 0;
        for (std::string line; std::getline(file, line); )
        {
            actions[i].first = line[0];
            actions[i].second = std::stoi(line.substr(2));
            ++i;
        }
        file.close();
    }
    std::array<std::pair<int, int>, 10> knots;
    std::unordered_set<std::pair<int, int>, Hash> poses;

    for (auto& ac : actions) {
        for (int _ = 0; _ < ac.second; _++) {
            auto& head = knots[0];
            if (ac.first == 'U')      { head.second++; }
            else if (ac.first == 'D') { head.second--; }
            else if (ac.first == 'R') { head.first++; }
            else if (ac.first == 'L') { head.first--; }
            else { std::cout << "ERROR"; std::cin.get(); }

            for (int i = 1; i < knots.size(); ++i) {
                auto& prev = knots[i - 1];
                auto& cur = knots[i];

                int dx = prev.first - cur.first;
                int dy = prev.second - cur.second;

                if (std::abs(dx) > 1) {
                    if (prev.first > cur.first) { cur.first++; }
                    else { cur.first--; }
                    if (std::abs(dy) > 0) {
                        if (prev.second > cur.second) { cur.second++; }
                        else { cur.second--; }
                    }
                } else if (std::abs(dy) > 1) {
                    if (prev.second > cur.second) { cur.second++; }
                    else { cur.second--; }
                    if (std::abs(dx) > 0) {
                        if (prev.first > cur.first) { cur.first++; }
                        else { cur.first--; }
                    }
                }
            }

            poses.insert(knots[9]);

            //if (std::abs(tail_x - head_x) > 1 || std::abs(tail_y - head_y) > 1) { std::cout << "ER " << tail_x << ' ' << tail_y << ' ' << head_x << ' ' << head_y << ' ' << ac.first; std::cin.get(); }
        }
    }

    int count = 0;
    count = (int)poses.size();
    std::cout << count;
}
