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

enum class TURN : char
{
    RIGHT,
    LEFT,
    NOTHING
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
    TURN turn;
    int times;
};

enum class BLOCK : char
{
    NOTHING,
    AIR,
    STONE
};

enum class WHERE : char
{
    TOP,
    LEFT,
    RIGHT,
    BACK,
    FRONT,
    BOTTOM
};

constexpr int SIDE = 50;

constexpr size_t WIDTH = 3 * SIDE;
constexpr size_t HEIGHT = 4 * SIDE;

void day22_2() // only works if the input is placed as my input was. 
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
            TURN turn = TURN::NOTHING;
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
                                moves.push_back(Move{ turn, std::stoi(line.substr(i)) });
                                get_new_num = false;
                            }
                        }
                        else {
                            get_new_num = true;
                            if (c == 'R') { turn = TURN::RIGHT; }
                            else if (c == 'L') { turn = TURN::LEFT; }
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

    WHERE w = WHERE::TOP;

    DIR dir = DIR::RIGHT;

    for (auto& move : moves)
    {
        
        if (move.turn == TURN::LEFT) { dir = turn_left(dir); }
        else if (move.turn == TURN::RIGHT) { dir = turn_right(dir); }

        for (int _ = 0; _ < move.times; _++)
        {
            int x_change = 0;
            int y_change = 0;

            if (dir == DIR::DOWN) { y_change = 1; }
            if (dir == DIR::RIGHT) { x_change = 1; }
            if (dir == DIR::LEFT) { x_change = -1; }
            if (dir == DIR::UP) { y_change = -1; }

            auto prev_x = x;
            auto prev_y = y;
            auto prev_dir = dir;
            auto prev_w = w;

            x += x_change;
            y += y_change;

            if (w == WHERE::TOP) {
                if (y < 0) {
                    y = 150 + (x - 50);
                    x = 0;
                    dir = DIR::RIGHT;
                    w = WHERE::FRONT;
                }
                else if (y >= 50) {
                    y = 50;

                    dir = DIR::DOWN;
                    w = WHERE::BACK;
                }
                else if (x < 50) {

                    x = 0;
                    y = 149 - y;

                    dir = DIR::RIGHT;
                    w = WHERE::LEFT;
                }
                else if (x >= 100) {
                    w = WHERE::RIGHT;
                }
            }
            else if (w == WHERE::RIGHT) {
                if (y < 0) {
                    y = 199;
                    x = (x - 100);

                    dir = DIR::UP;
                    w = WHERE::FRONT;
                }
                else if (y >= 50) {

                    y = 50 + (x - 100);
                    x = 99;
                    
                    dir = DIR::LEFT;
                    w = WHERE::BACK;
                }
                else if (x < 100) { 
                    w = WHERE::TOP;
                }
                else if (x >= 150) {

                    y = 149 - y;
                    x = 99;

                    dir = DIR::LEFT;
                    w = WHERE::BOTTOM;
                }
            }
            else if (w == WHERE::BACK) {
                if (y < 50) {
                    w = WHERE::TOP;
                }
                else if (y >= 100) {
                    w = WHERE::BOTTOM;
                }
                else if (x < 50) {

                    x = y - 50;
                    y = 100;

                    dir = DIR::DOWN;
                    w = WHERE::LEFT;
                }
                else if (x >= 100) {

                    x = 100 + (y - 50);
                    y = 49;

                    dir = DIR::UP;
                    w = WHERE::RIGHT;
                }
            }
            else if (w == WHERE::BOTTOM) { // y >= 100, x >= 50
                if (y < 100) {
                    w = WHERE::BACK;
                }
                else if (y >= 150) { 

                    y = 150 + (x - 50);
                    x = 49;

                    dir = DIR::LEFT;
                    w = WHERE::FRONT;
                }
                else if (x < 50) {
                    w = WHERE::LEFT;
                }
                else if (x >= 100) {

                    x = 149;
                    y = 149 - y;

                    dir = DIR::LEFT;
                    w = WHERE::RIGHT;
                }
            }
            else if (w == WHERE::LEFT) { // y >= 100, x >= 0
                if (y < 100) {

                    y = 50 + x;
                    x = 50;
                    
                    dir = DIR::RIGHT;
                    w = WHERE::BACK;
                }
                else if (y >= 150) {
                    w = WHERE::FRONT;
                }
                else if (x < 0) {

                    y = 49 - (y - 100);
                    x = 50;
                    
                    dir = DIR::RIGHT;
                    w = WHERE::TOP;
                }
                else if (x >= 50) {
                    w = WHERE::BOTTOM;
                }
            }
            else if (w == WHERE::FRONT) { // y >= 150, x >= 0
                if (y < 150) {
                    w = WHERE::LEFT;
                }
                else if (y >= 200) {
                    x = x + 100;
                    y = 0;

                    dir = DIR::DOWN;
                    w = WHERE::RIGHT;
                }
                else if (x < 0) {

                    x = (y - 150) + 50;
                    y = 0;

                    dir = DIR::DOWN;
                    w = WHERE::TOP;
                }
                else if (x >= 50) {

                    x = (y - 150) + 50;
                    y = 149;

                    dir = DIR::UP;
                    w = WHERE::BOTTOM;
                }
            }

            if (world[y][x] == BLOCK::NOTHING) {
                std::cout << "error! out of bounds";
            }

            if (world[y][x] == BLOCK::STONE)
            {
                x = prev_x;
                y = prev_y;
                dir = prev_dir;
                w = prev_w;
                break;
            }
        }
    }

    std::cout << "RES: " << (1000 * (y + 1) + 4 * (x + 1) + (int)dir);


}