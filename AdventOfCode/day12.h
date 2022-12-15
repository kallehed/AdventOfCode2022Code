#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>
#include <unordered_set>
#include <utility>

struct Pos {
    int8_t x;
    int8_t y;
};
constexpr int8_t WIDTH = 66;
constexpr int8_t HEIGHT = 41;
//constexpr int8_t WIDTH = 8;
//constexpr int8_t HEIGHT = 5;
using Form = std::array<std::array<bool, WIDTH>, HEIGHT>;
using mapForm = std::array<std::array<char, WIDTH>, HEIGHT>;

int recur(Form& where_been, Pos wher, mapForm& map, int steps, Pos& end)
{
    int least_steps = INT_MAX;
    constexpr std::array<std::array<int8_t, 2>, 4> ways = { {{1,0},{0,-1},{0,1},{-1,0}} };
    if (wher.x == end.x && wher.y == end.y) { std::cout << steps << '\n'; return steps; }

    for (auto& way : ways) {
        Pos n_pos = { wher.x + way[0], wher.y + way[1] };
        if (n_pos.x >= 0 && n_pos.x < WIDTH && n_pos.y >= 0 && n_pos.y < HEIGHT) {
            if (!where_been[n_pos.y][n_pos.x] && ((map[n_pos.y][n_pos.x] - 1) <= map[wher.y][wher.x]) && ((map[n_pos.y][n_pos.x] + 2) >= map[wher.y][wher.x])) {
                where_been[n_pos.y][n_pos.x] = true;
                int took = recur(where_been, n_pos, map, steps + 1, end);
                if (took < least_steps) { least_steps = took; }
                where_been[n_pos.y][n_pos.x] = false;
            }
        }
    }

    return least_steps;
}

void day12_1()
{

    mapForm map;

    Pos start = { -1,-1 };
    Pos end = { -1, -1 };

    std::ifstream file("day12.txt");
    if (file.is_open())
    {
        int i = 0;
        for (std::string line; std::getline(file, line); )
        {
            for (int j = 0; j < line.size(); ++j) { map[i][j] = line[j]; if (line[j] == 'S') { start.x = j; start.y = i; } if (line[j] == 'E') { end.x = j; end.y = i; } }
           
            ++i;
        }
        file.close();
    }
    Form start_form = { false };

    start_form[start.y][start.x] = true;
    map[start.y][start.x] = 'a';

    map[end.y][end.x] = 'z';

    std::cout << recur(start_form, start, map, 0, end);
}
