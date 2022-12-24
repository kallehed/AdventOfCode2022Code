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

//constexpr int WIDTH = 6;
//constexpr int HEIGHT = 4;
constexpr int WIDTH = 120;
constexpr int HEIGHT = 25;

struct Pos {
    int x, y;
};

inline int64_t modulo(int64_t a, int64_t b) {
    const int64_t result = a % b;
    return result >= 0 ? result : result + b;
}

enum class STATE {
    EMPTY,
    BLIZZARD,
    PERSON
};

std::array<std::array<bool, WIDTH>, HEIGHT> downs = { false };
std::array<std::array<bool, WIDTH>, HEIGHT> ups = { false };
std::array<std::array<bool, WIDTH>, HEIGHT> rights = { false };
std::array<std::array<bool, WIDTH>, HEIGHT> lefts = { false };

// "day24.h" for more details.
// In this one I use global variables FTW.
// Not much else interesting. Standard stuff

int took(const Pos start, const Pos end, const int min_offset)
{
    std::vector<Pos> poses = { start };
    int64_t minute = min_offset;
    while (true)
    {
        std::array<std::array<STATE, WIDTH>, HEIGHT> world = { STATE::EMPTY };
        // move the blizzards, then get new positions
        for (int64_t y = 0; y < HEIGHT; ++y) {
            for (int64_t x = 0; x < WIDTH; ++x) {
                if (lefts[y][x]) {
                    world[y][modulo(x - minute, WIDTH)] = STATE::BLIZZARD;
                }
                if (rights[y][x]) {
                    world[y][modulo(x + minute, WIDTH)] = STATE::BLIZZARD;
                }
                if (ups[y][x]) {
                    world[modulo(y - minute, HEIGHT)][x] = STATE::BLIZZARD;
                }
                if (downs[y][x]) {
                    world[modulo(y + minute, HEIGHT)][x] = STATE::BLIZZARD;
                }
            }
        }

        for (int i = int(poses.size()) - 1; i >= 0; --i)
        {
            
            const Pos p = poses[i];
            if (p.y != -1 && p.y != HEIGHT) {
                if (world[p.y][p.x] != STATE::EMPTY) {
                    // delete this pos
                    poses.erase(poses.begin() + i);
                }
            }
            constexpr const std::array<std::array<int, 2>, 4> ways = { {{1,0},{0,1},{-1,0},{0,-1} } };
            for (const auto& way : ways)
            {
                const Pos n_pos = { p.x + way[0], p.y + way[1] };
                if (n_pos.x >= 0 && n_pos.x < WIDTH && n_pos.y >= 0 && n_pos.y < HEIGHT) {
                    if (world[n_pos.y][n_pos.x] == STATE::EMPTY)
                    {
                        if (n_pos.x == end.x && n_pos.y == end.y) {
                            std::cout << "Took " << (minute + 1 - min_offset) << '\n';
                            return minute + 1;
                        }
                        poses.push_back(Pos{ n_pos.x, n_pos.y });
                        world[n_pos.y][n_pos.x] = STATE::PERSON;
                    }
                }
            }
        }
        ++minute;
    }
}

void day24_2()
{
    {
        std::ifstream file("day24.txt");
        if (file.is_open())
        {
            int y = -1;
            for (std::string line; std::getline(file, line); )
            {
                if (y < HEIGHT && y != -1) {
                    for (int i = 0; i + 1 < line.size(); ++i) {
                        const char c = line[i];
                        int x = i - 1;
                        if (c == '>') {
                            rights[y][x] = true;
                        }
                        else if (c == '^') {
                            ups[y][x] = true;
                        }
                        else if (c == '<') {
                            lefts[y][x] = true;
                        }
                        else if (c == 'v') {
                            downs[y][x] = true;
                        }
                    }
                }
                ++y;
            }
            file.close();
        }
    }

    int first = took({ 0,-1 }, { WIDTH - 1 , HEIGHT - 1 }, 0);
    int sec = took({ WIDTH - 1, HEIGHT }, { 0,0 }, first + 1);
    int finals = took({ 0,-1 }, { WIDTH - 1 , HEIGHT - 1 }, sec + 1);

    std::cout << "TOTAL: " << finals;
   

}