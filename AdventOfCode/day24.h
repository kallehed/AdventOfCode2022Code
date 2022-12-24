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
    int8_t x, y;
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

// Very epic solution actually
// It has the input data of blizzard positions, and uses modulo to each "round" make a complete "state-matrix" of where you can and can't go using the current minute.
// Then I do a breadth-first-search (BFS) from the start position, adding positions that you can walk to, deleting positions that are invalid
// I have a vector of the positions, and to avoid duplicates I use this method: When a new position is added,
// it will mark that position in the "state-matrix" as already being gone to.
// Any other posititon that could have gone to the same position, will check if it is empty, and if not, will not go there.
// Therefore, there will only be unique positons in the vector.
// This ensures that there can only be WIDTH*HEIGHT ( plus one position that starts outside) positions in total;
// which means this is incredibly fast. Still, there could possibly be a better way to handle the blizzard representations.
// Maybe actually moving the blizzards around could be faster? Also, I currently use a lot of modulo.


void day24_1()
{
    std::array<std::array<bool, WIDTH>, HEIGHT> downs = {false};
    std::array<std::array<bool, WIDTH>, HEIGHT> ups = {false};
    std::array<std::array<bool, WIDTH>, HEIGHT> rights = {false};
    std::array<std::array<bool, WIDTH>, HEIGHT> lefts = {false};

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

    std::vector<Pos> poses = { {0, -1} };
    int64_t minute = 0;
    while (true)
    {
        std::array<std::array<STATE, WIDTH>, HEIGHT> world = {STATE::EMPTY};
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
            constexpr const std::array<std::array<int8_t, 2>, 4> ways = { {{1,0},{0,1},{-1,0},{0,-1} } };
            const Pos p = poses[i];
            if (p.y != -1) {
                if (world[p.y][p.x] != STATE::EMPTY) {
                    // delete this pos
                    poses.erase(poses.begin() + i);
                }
            }

            for (const auto& way : ways)
            {
                const Pos n_pos = { p.x + way[0], p.y + way[1] };
                if (n_pos.x >= 0 && n_pos.x < WIDTH && n_pos.y >= 0 && n_pos.y < HEIGHT) {
                    if (world[n_pos.y][n_pos.x] == STATE::EMPTY)
                    {
                        if (n_pos.x == WIDTH - 1 && n_pos.y == HEIGHT - 1) {
                            std::cout << "Took " << (minute + 1) << '\n';
                            std::cin.get();
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