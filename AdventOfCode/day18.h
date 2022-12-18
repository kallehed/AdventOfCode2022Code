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

struct Cube {
    int x, y, z;
    bool operator== (const Cube& c) {
        return x == c.x && y == c.y && z == c.z;
    }
};

void day18_1()
{
    std::vector<Cube> cubes;
    {
        std::ifstream file("day18.txt"); // get names of all valves
        if (file.is_open())
        {
            for (std::string line; std::getline(file, line); )
            {
                Cube c;
                c.x = std::stoi(line);
                auto first_comma = line.find(',');
                c.y = std::stoi(line.substr(first_comma + 1));
                auto second_comma = line.substr(first_comma + 1).find(',') + first_comma + 1;
                c.z = std::stoi(line.substr(second_comma + 1));

                cubes.push_back(c);
            }

            file.close();
        }
    }
    
    int total_area = 0;
    for (const auto& c : cubes) {
        constexpr std::array<std::array<int, 3>, 6> ways = { {{1,0,0},{-1,0,0},{0,1,0},{0,-1,0},{0,0,1},{0,0,-1}} };

        for (const auto& way : ways) {
            Cube n_cube = { c.x + way[0], c.y + way[1], c.z + way[2] };
            if (std::find(cubes.begin(), cubes.end(), n_cube) == cubes.end()) {
                ++total_area;
            }
            
        }
    }
    std::cout << "Total area: " << total_area;
}

constexpr int MAX_LEN = 22;
//constexpr int MAX_LEN = 6;
constexpr int ACTUAL_LEN = MAX_LEN + 5;

enum class STATE {
    NOT_LOOKED_AT_AIR,
    LAVA,
    LOOKED_AT_AIR,
};

constexpr std::array<std::array<int, 3>, 6> ways = { {{1,0,0},{-1,0,0},{0,1,0},{0,-1,0},{0,0,1},{0,0,-1}} };

void go(std::array<std::array<std::array<STATE, ACTUAL_LEN>, ACTUAL_LEN>, ACTUAL_LEN>& world, int& total_area, const Cube& pos)
{
    if (pos.x == 0 || pos.x + 1 >= ACTUAL_LEN || pos.y == 0 || pos.y + 1 >= ACTUAL_LEN || pos.z == 0 || pos.z + 1 >= ACTUAL_LEN) {
        return;
    }
    if (world[pos.x][pos.y][pos.z] != STATE::NOT_LOOKED_AT_AIR) { return; }
    world[pos.x][pos.y][pos.z] = STATE::LOOKED_AT_AIR;
    
    for (const auto& way : ways) {
        //Cube new_pos = { pos.x + way[0], pos.y + way[1], pos.z + way[2] };

        if (world[pos.x + way[0]][pos.y + way[1]][pos.z+way[2]] == STATE::LAVA) {
            ++total_area;
        }
    }
    
    for (const auto& way : ways) {
        go(world, total_area, { pos.x + way[0], pos.y + way[1], pos.z + way[2] });
    }

}

void day18_2() // for this solution I recursively go through all positions and look at the lava that is near the air that the water is able to get to. 
{
    std::array<std::array<std::array<STATE, ACTUAL_LEN>, ACTUAL_LEN>, ACTUAL_LEN> world;
    for (auto& a : world) {
        for (auto& b : a) {
            for (auto& c : b) {
                c = STATE::NOT_LOOKED_AT_AIR;
            }
        }
    }

    {
        std::ifstream file("day18.txt"); // get names of all valves
        if (file.is_open())
        {
            for (std::string line; std::getline(file, line); )
            {
                Cube c;
                c.x = std::stoi(line);
                auto first_comma = line.find(',');
                c.y = std::stoi(line.substr(first_comma + 1));
                auto second_comma = line.substr(first_comma + 1).find(',') + first_comma + 1;
                c.z = std::stoi(line.substr(second_comma + 1));

                world[c.x + 2][c.y + 2][c.z + 2] = STATE::LAVA;
            }

            file.close();
        }
    }
    int total_area = 0;

    go(world, total_area, { 1,1,1 });

    std::cout << "Total area: " << total_area;
}