#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>
#include <unordered_set>
#include <utility>

void check_cycle(long long cycle, long long* X, long long* total) {
    if ((cycle + 20) % 40 == 0) {
        std::cout << "cycle: " << cycle << ", X: " << *X << ", ADD: " << cycle*(*X) << '\n';
        *total += cycle * (*X);
    }
}

void day10_1()
{
    std::cout << "doing";
    std::ifstream file("day10.txt");
    if (file.is_open()) {

        long long total_sign_stren = 0;
        long long cycle = 1;
        long long X = 1;
        for (std::string line; std::getline(file, line); )
        {
            if (line[0] == 'a') {
                long long V = std::stoll(line.substr(5));
                cycle += 1;
                check_cycle(cycle, &X, &total_sign_stren);
                cycle += 1;
                X += V;
                check_cycle(cycle, &X, &total_sign_stren);
                
            }
            else {
                cycle += 1;
                check_cycle(cycle, &X, &total_sign_stren);
            }
        }

        std::cout << "tot sig stren: " << total_sign_stren;

        file.close();
    }
}

void check_cycle_2(int* x, int* y, long long* X, std::array <std::array<bool, 40>, 7>* image) {
    if (*x == 39) { (* x) = 0; (* y) = (*y) + 1; }
    else { (*x)++; }
    bool draw = (std::abs((*x) - (int)(*X)) < 2);
    (*image)[*y][*x] = draw;
}

void day10_2()
{
    //std::cout << "doing";
    std::ifstream file("day10.txt");
    if (file.is_open()) {

        std::array <std::array<bool, 40>, 7> image = {false};
        int x = 0, y = 0;

        long long total_sign_stren = 0;
        long long cycle = 1;
        long long X = 1;
        for (std::string line; std::getline(file, line); )
        {
            if (line[0] == 'a') {
                long long V = std::stoll(line.substr(5));
                cycle += 1;
                check_cycle_2(&x, &y, &X, &image);
                cycle += 1;
                X += V;
                check_cycle_2(&x, &y, &X, &image);

            }
            else {
                cycle += 1;
                check_cycle_2(&x, &y, &X, &image);
            }
        }

        for (auto& row: image) {
            for (auto& cell : row) {
                std::cout << (cell ? '#' : '.');
            }
            std::cout << '\n';
        }

        file.close();
    }
}