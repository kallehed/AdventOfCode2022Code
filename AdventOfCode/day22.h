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

enum class DIR : char
{
    RIGHT,
    DOWN,
    LEFT,
    UP,
};

DIR turn_right(DIR d) {
    return (DIR)((((char)d) + 1) % 4);
}

DIR turn_left(DIR d) {
    char n = (char)d - 1;
    if (n < 0) { return DIR::UP; }
    return (DIR)n;
}

struct Move {
    DIR dir;
    int times;
};

enum class BLOCK : char
{
    NOTHING,
    AIR,
    STONE
};

constexpr int SIDE = 50;

constexpr size_t WIDTH = 3 * SIDE;
constexpr size_t HEIGHT = 4 * SIDE;


void day22_1()
{

    std::array<std::array<BLOCK, WIDTH>, HEIGHT> world;
    for (auto& c : world) {
        for (auto& a : c) {
            a = BLOCK::NOTHING;
        }
    }

    std::vector<Move> moves;

    {
        std::ifstream file("day22.txt");
        if (file.is_open())
        {
            bool first_mode = true;
            int y = 0;
            DIR dir = DIR::RIGHT;
            for (std::string line; std::getline(file, line); )
            {
                if (line.length() == 0) {
                    first_mode = false;
                    continue;
                }

                if (first_mode)
                {
                    for (int i = 0; i < line.size(); ++i) {
                        char c = line[i];

                        if (c != ' ') {
                            if (c == '.') {
                                world[y][i] = BLOCK::AIR;
                            }
                            else {
                                world[y][i] = BLOCK::STONE;
                            }
                        }
                    }
                    ++y;
                }
                else {
                    bool get_new_num = true;
                    for (int i = 0; i < line.size(); ++i) {
                        char c = line[i];
                        if (c >= '0' && c <= '9') {
                            if (get_new_num) {
                                moves.push_back(Move{ dir, std::stoi(line.substr(i)) });
                                get_new_num = false;
                            }
                        }
                        else {
                            get_new_num = true;
                            if (c == 'R') { dir = turn_right(dir); }
                            else if (c == 'L') { dir = turn_left(dir); }
                        }
                    }
                }
            }
            file.close();
        }
    }
    int x = 0, y = 0;
    for (;; ++x) {
        if (world[y][x] == BLOCK::AIR) { break; }
    }


    for (auto& move : moves) 
    {
        int x_change = 0;
        int y_change = 0;

        if (move.dir == DIR::DOWN) { y_change = 1; }
        if (move.dir == DIR::RIGHT) { x_change = 1; }
        if (move.dir == DIR::LEFT) { x_change = -1; }
        if (move.dir == DIR::UP) { y_change = -1; }

        for (int _ = 0; _ < move.times; _++)
        {
            x += x_change;
            y += y_change;
            bool walked_into_stone = false;
            while (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT || world[y][x] != BLOCK::AIR) {
                if (x < 0) { x = WIDTH - 1; }
                else if (y < 0) { y = HEIGHT - 1; }
                else if (x >= WIDTH) { x = 0; }
                else if (y >= HEIGHT) { y = 0; }
                else {
                    if (world[y][x] == BLOCK::STONE) {
                        x_change *= -1;
                        y_change *= -1;
                        walked_into_stone = true;
                    }

                    x += x_change;
                    y += y_change;
                }
            }
            if (walked_into_stone) { break; }
        }
    }

    std::cout << "RES: " << (1000 * (y + 1) + 4 * (x + 1) + (int)moves[moves.size() - 1].dir);


}