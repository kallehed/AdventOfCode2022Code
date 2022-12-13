#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>

void day9_1()
{

    int head_x = 0;
    int head_y = 0;
    int tail_x = 0;
    int tail_y = 0;

    std::array<std::array<bool, 2000>, 2000> matrix = { {false} };


    std::ifstream file("day9.txt");
    if (file.is_open()) {
        std::cout << "pos: " << tail_x << " " << tail_y << '\n';
        for (std::string line; std::getline(file, line); )
        {
            int times = std::stoi(line.substr(2));
            char action = line[0];
            std::cout << "action: " << action << " " << times << '\n';

            for (int _ = 0; _ < times; ++_) {
                matrix[tail_y + 1000][tail_x + 1000] = true;
                if (action == 'R') { ++head_x; }
                if (action == 'L') { --head_x; }
                if (action == 'U') { ++head_y; }
                if (action == 'D') { --head_y; }

                int old_head_x = head_x, old_head_y = head_y;

                if (std::abs(head_x - tail_x) >= 2) {
                    if (head_x > tail_x) { ++tail_x; }
                    else { --tail_x; }
                    if (std::abs(head_y - tail_y) >= 1) {
                        
                        if (head_y > tail_y) { ++tail_y; }
                        else { --tail_y; }
                    }
                }
                else if (std::abs(head_y - tail_y) >= 2) {
                    if (head_y > tail_y) { ++tail_y; }
                    else { --tail_y; }
                    if (std::abs(head_x - tail_x) >= 1) {

                        if (head_x > tail_x) { ++tail_x; }
                        else { --tail_x; }
                    }
                }
                std::cout << "pos: " << tail_x << " " << tail_y << '\n';

                
            }
            
        }
        file.close();
    }

    int count = 0;
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[i].size(); ++j) {
            if (matrix[i][j]) ++count;
        }
    }
    std::cout << count;


}
