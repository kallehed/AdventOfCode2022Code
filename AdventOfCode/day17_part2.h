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

enum class DIR : char {
    LEFT,
    RIGHT
};

enum class GRID : char {
    AIR,
    ROCK
};

using INT = long long;

struct Pos {
    INT x;
    INT y;
    bool operator==(const Pos& rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }
};

constexpr INT WIDTH = 7;
constexpr INT HEIGHT = 100LL;

//constexpr INT ROCKS_TO_PLACE = 2022LL;
constexpr INT ROCKS_TO_PLACE = (100910 + 9549340);


// ehmm this isn't actually a solution, but I basically tried to find the patterns manually, and realized it repeated at some points, and then I manually calculated the total stones based on that lol
// One would have to write an algorithm that finds when it repeats.

void day17_2() 
{
    std::vector<DIR> directions;
    {
        std::ifstream file("day17.txt"); // get names of all valves
        if (file.is_open())
        {
            for (std::string line; std::getline(file, line); )
            {
                for (auto& c : line) {
                    switch (c) {
                    case '<':
                        directions.push_back(DIR::LEFT);
                        break;
                    case '>':
                        directions.push_back(DIR::RIGHT);
                    }
                }
            }

            file.close();
        }
    }

    std::array<std::array<GRID, WIDTH>, HEIGHT> grid;
    for (auto& a : grid) {
        for (auto& e : a) {
            e = GRID::AIR;
        }
    }

    const std::vector<std::vector<Pos>> formations = { {{0,0},{1,0},{2,0},{3,0}}, {{1,0},{0,1},{1,1},{2,1},{1,2}}, {{0,0},{1,0},{2,0},{2,1},{2,2}}, {{0,0},{0,1},{0,2},{0,3}},{ {0, 0}, { 1,0 }, { 0,1 }, { 1,1 }} };

    //constexpr const std::array<const int, 5> height_of_formation = { { 2, 4, 3, 3, 1 } };

    std::vector<Pos> cur;

    INT rocks_placed = 0;
    bool new_form = true;
    int what_form = (int)formations.size() - 1;
    size_t dir_index = 0;
    INT highest_y = -1;
    INT total_highest_y = 0;
    while (rocks_placed < ROCKS_TO_PLACE)
    {
        // print
        /* {
            if constexpr (false) {
                for (int y = 10; y >= 0; --y) {
                    for (int x = 0; x < WIDTH; ++x) {
                        Pos p = { x,y };
                        char c = '.';
                        if (std::find(cur.begin(), cur.end(), p) != cur.end()) {
                            c = '@';
                        }
                        else if (grid[y][x] == GRID::ROCK) { c = '#'; }
                        std::cout << c;
                    }
                    std::cout << '\n';
                }
                std::cin.get();
            }
        }*/


        if (new_form) {
            cur.clear();
            what_form = (what_form + 1) % ((int)formations.size());

            {
                // move everything down if it gets too high
                if (highest_y + 8LL >= HEIGHT)
                {
                    constexpr INT mid = HEIGHT / 2;
                    
                    for (INT y = 0; y < mid; ++y) {
                        for (INT x = 0; x < WIDTH; ++x) {
                            grid[y][x] = grid[mid + y][x];
                            grid[mid + y][x] = GRID::AIR;
                        }
                    }

                    highest_y -= mid;
                    total_highest_y += mid;
                }
                if (highest_y != -1) {
                    // check highest y for reacurrance
                    bool cool = true;
                    /*for (INT x = 0; x < WIDTH; ++x) {
                        if (grid[highest_y][x] == GRID::AIR) {
                            cool = false;
                        }
                    }*/
                    if ((rocks_placed - 100910 ) % (100910 * 175) == 0) {
                        std::cout << "rocks: " << rocks_placed << " ";
                        for (INT x = 0; x < WIDTH; ++x) {
                            std::cout << ((grid[highest_y][x] == GRID::ROCK) ? ('#') : ('.'));
                        }
                        
                        std::cout << " highest: " << (total_highest_y + highest_y + 1) << '\n';
                    }
                    if (cool) {

                    }
                }
                
            }

            for (auto& pos : formations[what_form]) {

                cur.emplace_back(pos.x + 2LL, pos.y + highest_y + 4LL);
            }

            new_form = false;
            continue;
        }

        // move a direction
        {
            DIR& dir = directions[dir_index];
            dir_index = (dir_index + 1) % directions.size();
            const INT x_move = (dir == DIR::LEFT) ? (-1LL) : (1LL);

            // check if it is okay
            bool legal_move = true;
            for (auto& pos : cur) {
                const INT new_x = pos.x + x_move;
                if (new_x < 0LL || new_x >= WIDTH || grid[pos.y][new_x] == GRID::ROCK) {
                    // illegal position
                    legal_move = false;
                    break;
                }
            }
            // if ILLEGAL move => do nothing, else move
            if (legal_move) {
                for (auto& pos : cur) {
                    pos.x += x_move;
                }
            }
        }

        // fall a step
        {
            constexpr INT y_move = -1;

            // check if it is okay
            bool legal_move = true;
            for (auto& pos : cur) {
                const INT new_y = pos.y + y_move;

                if (new_y < 0 || grid[new_y][pos.x] == GRID::ROCK) {
                    legal_move = false;
                    break;
                }
            }
            if (legal_move) {
                for (auto& pos : cur) {
                    pos.y += y_move;
                }
            }
            else {
                // can't go down, therefore must settle
                for (const auto& pos : cur) {
                    grid[pos.y][pos.x] = GRID::ROCK;
                    if (pos.y > highest_y) { highest_y = pos.y; }
                }
                new_form = true;
                ++rocks_placed;
                continue;
            }
        }
    }
    std::cout << "Rocks placed: " << rocks_placed << " highest y: " << (total_highest_y + highest_y + 1);
}