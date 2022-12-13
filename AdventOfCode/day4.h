#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

void day4_1()
{
    int total = 0;

    std::ifstream file("day4.txt");
    if (file.is_open()) {
        for (std::string line; std::getline(file, line); )
        {
            size_t comma = line.find(',');
            size_t dash1 = line.find('-');
            size_t dash2 = line.find('-', comma + 1);
            //std::cout << comma << ' ' << dash1 << ' ' << dash2 << '\n';
            int first_start = std::stoi(line);
            int first_end = std::stoi(line.substr(dash1 + 1));
            int second_start = std::stoi(line.substr(comma + 1));
            int second_end = std::stoi(line.substr(dash2 + 1));
            
            if ((first_start >= second_start && first_end <= second_end) || (second_start >= first_start && second_end <= first_end)) {
                total += 1;
            }

        }
        file.close();
    }
    std::cout << total;
}

void day4_2()
{
    int total = 0;

    std::ifstream file("day4.txt");
    if (file.is_open()) {
        for (std::string line; std::getline(file, line); )
        {
            size_t comma = line.find(',');
            size_t dash1 = line.find('-');
            size_t dash2 = line.find('-', comma + 1);
            //std::cout << comma << ' ' << dash1 << ' ' << dash2 << '\n';
            int first_start = std::stoi(line);
            int first_end = std::stoi(line.substr(dash1 + 1));
            int second_start = std::stoi(line.substr(comma + 1));
            int second_end = std::stoi(line.substr(dash2 + 1));

            if ((first_start >= second_start && first_start <= second_end) || ((first_end >= second_start && first_end <= second_end)) ||
                (second_start >= first_start && second_start <= first_end) || ((second_end >= first_start && second_end <= first_end))) {
                total += 1;
            }

        }
        file.close();
    }
    std::cout << total;
}

