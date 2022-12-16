#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>
#include <unordered_set>
#include <utility>

enum class BLOCK : char {
    AIR,
    STONE,
    REST_SAND,
};

constexpr int HEIGHT = 1000;
constexpr int WIDTH = 1000;
using Form = std::array<std::array<BLOCK, WIDTH>, HEIGHT>;

void print_world(Form& world, int start_x, int end_x, int start_y, int end_y) {
    for (int y = start_y; y <= end_y; ++y) {
        for (int x = start_x; x <= end_x; ++x) {
            std::cout << (world[y][x] == BLOCK::AIR ? '.' : (world[y][x] == BLOCK::STONE ? '#' : 'O'));
        }
        std::cout << '\n';
    }
}

void day14_1()
{

    Form world;

    for (auto& row : world) {
        for (auto& block : row) {
            block = BLOCK::AIR;
        }
    }

    std::ifstream file("day14.txt");
    if (file.is_open())
    {

        for (std::string line; std::getline(file, line); )
        {
            int i = 0;
            bool can_read_numbers = true;
            bool on_first = true;
            bool on_x = true;
            int prev_x = -1;
            int prev_y = -1;
            int next_x = -1;
            int next_y = -1;
            while (true)
            {
                if (i >= line.size()) {
                    can_read_numbers = false;
                    goto put_blocks;
                }

                if (can_read_numbers && line[i] >= '0' && line[i] <= '9') {
                    int num = std::stoi(line.substr(i));
                    can_read_numbers = false;
                    if (on_first) {
                        if (on_x) {
                            prev_x = num;
                        }
                        else {
                            prev_y = num;
                        }
                    }
                    else {
                        if (on_x) {
                            next_x = num;
                        }
                        else {
                            next_y = num;
                        }
                    }
                }
                else if (line[i] == ',') {
                    can_read_numbers = true;
                    on_x = false;
                }
                else if (line[i] == '>') {
                    on_x = true;
                    can_read_numbers = true;
                    if (on_first) {
                        on_first = false;
                    }
                    else {
                        put_blocks:
                        // add path from prev to next

                        int dx = next_x - prev_x;
                        int dy = next_y - prev_y;

                        int steps = std::max(std::abs(dx), std::abs(dy));
                        int x_move = dx / (std::max(1, std::abs(dx)));
                        int y_move = dy / (std::max(1, std::abs(dy)));

                        for (int j = 0; j < steps; ++j) {
                            world[prev_y][prev_x] = BLOCK::STONE;
                            prev_x += x_move;
                            prev_y += y_move;
                        }
                        world[prev_y][prev_x] = BLOCK::STONE;
                        if (!can_read_numbers) { break; }
                    }
                }
                ++i;
            }
        }

        // write out map of stone
        print_world(world, 494, 503, 0, 9);

        // simulate sand
        int total_sand = 0;
        int x = 500;
        int y = 0;
        while (true)
        {
            // move
            if (y + 1 >= HEIGHT)
            {
                // fallen through
                break;
            }
            if (world[y + 1][x] != BLOCK::AIR) {
                if (world[y + 1][x - 1] != BLOCK::AIR) {
                    if (world[y + 1][x + 1] != BLOCK::AIR) {
                        // rest here
                        world[y][x] = BLOCK::REST_SAND;
                        ++total_sand;
                        x = 500;
                        y = 0;
                    }
                    else {
                        ++y; ++x;
                    }
                }
                else {
                    ++y; --x;
                }
            }
            else {
                ++y;
            }
        }

        std::cout << "Total sands: " << total_sand;

        file.close();
    }

}

void day14_2()
{

    Form world;

    for (auto& row : world) {
        for (auto& block : row) {
            block = BLOCK::AIR;
        }
    }
    

    std::ifstream file("day14.txt");
    if (file.is_open())
    {
        int highest_y = -1;
        for (std::string line; std::getline(file, line); )
        {
            int i = 0;
            bool can_read_numbers = true;
            bool on_first = true;
            bool on_x = true;
            int prev_x = -1;
            int prev_y = -1;
            int next_x = -1;
            int next_y = -1;
            
            while (true)
            {
                if (i >= line.size()) {
                    can_read_numbers = false;
                    goto put_blocks;
                }

                if (can_read_numbers && line[i] >= '0' && line[i] <= '9') {
                    int num = std::stoi(line.substr(i));
                    can_read_numbers = false;
                    if (on_first) {
                        if (on_x) {
                            prev_x = num;
                        }
                        else {
                            prev_y = num;
                            if (prev_y > highest_y) { highest_y = prev_y; }
                        }
                    }
                    else {
                        if (on_x) {
                            next_x = num;
                        }
                        else {
                            next_y = num;
                            if (next_y > highest_y) { highest_y = next_y; }
                        }
                    }
                }
                else if (line[i] == ',') {
                    can_read_numbers = true;
                    on_x = false;
                }
                else if (line[i] == '>') {
                    on_x = true;
                    can_read_numbers = true;
                    if (on_first) {
                        on_first = false;
                    }
                    else {
                    put_blocks:
                        // add path from prev to next

                        int dx = next_x - prev_x;
                        int dy = next_y - prev_y;

                        int steps = std::max(std::abs(dx), std::abs(dy));
                        int x_move = dx / (std::max(1, std::abs(dx)));
                        int y_move = dy / (std::max(1, std::abs(dy)));

                        for (int j = 0; j < steps; ++j) {
                            world[prev_y][prev_x] = BLOCK::STONE;
                            prev_x += x_move;
                            prev_y += y_move;
                        }
                        world[prev_y][prev_x] = BLOCK::STONE;
                        if (!can_read_numbers) { break; }
                    }
                }
                ++i;
            }
        }

        highest_y += 2;
        for (int x = 0; x < WIDTH; ++x) { // floor
            world[highest_y][x] = BLOCK::STONE;
        }

        // write out map of stone
        std::cout << "highest_y: " << highest_y << '\n';
        print_world(world, 450, 540, 0, 173);

        // simulate sand
        int total_sand = 0;
        int x = 500;
        int y = 0;
        while (true)
        {
            // move
            if (world[0][500] == BLOCK::REST_SAND) 
            {
                // fallen through
                break;
            }
            if (world[y + 1][x] != BLOCK::AIR) {
                if (world[y + 1][x - 1] != BLOCK::AIR) {
                    if (world[y + 1][x + 1] != BLOCK::AIR) {
                        // rest here
                        world[y][x] = BLOCK::REST_SAND;
                        ++total_sand;
                        x = 500;
                        y = 0;
                    }
                    else {
                        ++y; ++x;
                    }
                }
                else {
                    ++y; --x;
                }
            }
            else {
                ++y;
            }
        }

        std::cout << "Total sands: " << total_sand;

        file.close();
    }

}