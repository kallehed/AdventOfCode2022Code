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
using Form = std::array<std::array<int, WIDTH>, HEIGHT>;
using mapForm = std::array<std::array<char, WIDTH>, HEIGHT>;

void print_where_been(Form& where_been) {
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            std::cout << where_been[i][j];
        }
        std::cout << '\n';
    }
}

void recur(Form& where_been, Pos wher, mapForm& map, Pos& end, int steps) // basically djikstras algorithm
{
    if (steps < where_been[wher.y][wher.x]) { 
        where_been[wher.y][wher.x] = steps; }
    else {
        return;
    }

    constexpr std::array<Pos, 4> ways = { { {1,0},{0,1},{0,-1},{-1,0} } };

    for (auto& way : ways) {
        Pos n_pos = { wher.x + way.x, wher.y + way.y };
        if (n_pos.x >= 0 && n_pos.x < WIDTH && n_pos.y >= 0 && n_pos.y < HEIGHT) {
            if ((map[wher.y][wher.x] + 1 >= map[n_pos.y][n_pos.x])) {
                recur(where_been, n_pos, map, end, steps+1);
            }
        }
    }

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
    Form start_form;
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            start_form[i][j] = INT_MAX;
        }
    }

    map[start.y][start.x] = 'a';

    map[end.y][end.x] = 'z';

    recur(start_form, start, map, end, 0);
    std::cout << "res: " << start_form[end.y][end.x];
}

void day12_2()
{

    mapForm map;

    Pos end = { -1, -1 };

    std::vector<Pos> start_poses;

    std::ifstream file("day12.txt");
    if (file.is_open())
    {
        int i = 0;
        for (std::string line; std::getline(file, line); )
        {
            for (int j = 0; j < line.size(); ++j) { map[i][j] = line[j]; if (line[j] == 'S' || line[j] == 'a') { start_poses.push_back(Pos{ (char)j,(char)i }); map[i][j] = 'a'; } if (line[j] == 'E') { end.x = j; end.y = i; } }

            ++i;
        }
        file.close();
    }

    int least = INT_MAX;

    for (auto& st : start_poses) {
        Form start_form;
        for (int i = 0; i < HEIGHT; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                start_form[i][j] = INT_MAX;
            }
        }

        map[end.y][end.x] = 'z';

        recur(start_form, st, map, end, 0);
        int l = start_form[end.y][end.x];
        std::cout << "res: " << l;
        if (l < least) { least = l; }
    }
    std::cout << '\n' << "FINAL: " << least;
}
