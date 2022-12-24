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

//constexpr size_t WIDTH = 14; // for the example data
//constexpr size_t HEIGHT = 12;
//onstexpr size_t OFFSET = 6;

constexpr size_t WIDTH = 71; // width of my data
constexpr size_t HEIGHT = 71; // height of my data
constexpr size_t OFFSET = 200; // some offset which has to be even, which adds extra room to the width and height, so the elves have more wiggle room

constexpr size_t REAL_WIDTH = WIDTH + OFFSET;
constexpr size_t REAL_HEIGHT = HEIGHT + OFFSET;

enum class SPACE : char {
    AIR,
    ELF,
    ELF_WANTS_TO_GO_HERE,
    TWO_ELFS_WANT_TO_GO_HERE,
};

struct Elf_Move {
    int start_y, start_x, end_y, end_x;
};

void day23_2()
{
    std::array<std::array<SPACE, REAL_WIDTH>, REAL_HEIGHT> world;
    for (auto& a : world) {
        for (auto& b : a) {
            b = SPACE::AIR;
        }
    }
    {
        std::ifstream file("day23.txt");
        if (file.is_open())
        {
            size_t y = 0;
            for (std::string line; std::getline(file, line); )
            {
                for (int i = 0; i < line.size(); ++i) {
                    if (line[i] == '#') { world[y + OFFSET / 2][i + OFFSET / 2] = SPACE::ELF; }
                }
                ++y;
            }
            file.close();
        }
    }

    std::array<std::array<std::array<int, 2>, 3>, 4> moves;
    {
        std::array < std::array<int, 2>, 3> a1 = { { {-1,0 }, { -1,-1 }, { -1,1 } } };
        std::array < std::array<int, 2>, 3> a2 = { { {1,0},{1,1},{1,-1} } };
        std::array < std::array<int, 2>, 3> a3 = { {{0,-1},{-1,-1},{1,-1}} };
        std::array < std::array<int, 2>, 3> a4 = { {{0,1},{1,1},{-1,1}} };
        moves[0] = a1;
        moves[1] = a2;
        moves[2] = a3;
        moves[3] = a4;
    }

    std::vector<Elf_Move> elf_moves;

    for (int round = 0; true; ++round)
    {
        // print out
        if (false) {
            std::cout << "Round " << round << '\n';
            for (size_t y = 0; y < REAL_HEIGHT; ++y) {
                for (size_t x = 0; x < REAL_WIDTH; ++x) {
                    std::cout << ((world[y][x] == SPACE::ELF) ? '#' : (world[y][x] == SPACE::AIR ? '.' : '?'));
                }
                std::cout << '\n';
            }
            std::cin.get();
        }

        for (size_t y = 0; y  < REAL_HEIGHT; ++y) {
            for (size_t x = 0; x < REAL_WIDTH; ++x) {
                if (world[y][x] == SPACE::ELF) {
                    // move the elf
                    if (world[y + 1][x] != SPACE::ELF && world[y + 1][x + 1] != SPACE::ELF && world[y + 1][x - 1] != SPACE::ELF && world[y - 1][x] != SPACE::ELF && world[y - 1][x + 1] != SPACE::ELF && world[y - 1][x - 1] != SPACE::ELF && world[y][x + 1] != SPACE::ELF && world[y][x - 1] != SPACE::ELF) {
                        // do nothing
                    }
                    else {
                        for (int _ = 0; _ < 4; ++_) {
                            auto& move = moves[(round + _) % (int)moves.size()];

                            // move consists of 3 ways to go
                            // check all of them, if everyone works, go the first one of them
                            bool all_air = true;
                            for (auto& m : move) {
                                if (world[y + m[0]][x + m[1]] == SPACE::ELF) {
                                    all_air = false;
                                    break;
                                }
                            }
                            if (all_air) {
                                auto& w = world[y + move[0][0]][x + move[0][1]];
                                if (w == SPACE::ELF_WANTS_TO_GO_HERE) {
                                    w = SPACE::TWO_ELFS_WANT_TO_GO_HERE;
                                    break;
                                }
                                else if (w == SPACE::AIR) {
                                    w = SPACE::ELF_WANTS_TO_GO_HERE;
                                    // add to vector
                                    Elf_Move e_m;
                                    e_m.start_y = y;
                                    e_m.start_x = x;
                                    e_m.end_y = y + move[0][0];
                                    e_m.end_x = x + move[0][1];
                                    elf_moves.push_back(e_m);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        // execute all moves
        bool moved = false;
        for (const auto& elf_move : elf_moves) {
            auto& start = world[elf_move.start_y][elf_move.start_x];
            _ASSERT(start == SPACE::ELF);
            auto& going = world[elf_move.end_y][elf_move.end_x];
            _ASSERT(going != SPACE::AIR && going != SPACE::ELF);
            if (going == SPACE::ELF_WANTS_TO_GO_HERE) {
                going = SPACE::ELF;
                start = SPACE::AIR;
                moved = true;
            }
            else {
                _ASSERT(going == SPACE::TWO_ELFS_WANT_TO_GO_HERE);
                going = SPACE::AIR;
            }
        }
        if (!moved) { std::cout << "round: " << (round + 1); break; }
        elf_moves.clear();
    }
}