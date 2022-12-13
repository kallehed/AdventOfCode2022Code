#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>

void day8_1()
{
    std::vector<std::vector<char>> matrix;

    std::ifstream file("day8.txt");
    if (file.is_open()) {

        for (std::string line; std::getline(file, line); )
        {
            matrix.push_back({});
            for (char c : line) {
                matrix[matrix.size() - 1].push_back(c);
            }
        }
        file.close();
    }

    int total_visible = 0;

    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[i].size(); ++j) {
            char val = matrix[i][j];

            int visible = 4;

            for (int k = j - 1; k >= 0; --k) { // left
                if (matrix[i][k] >= val) { // higher, therefore not visible
                    visible -= 1;
                    break;
                }
            }
            for (int k = j + 1; k < matrix[i].size(); ++k) { // right
                if (matrix[i][k] >= val) {
                    --visible;
                    break;
                }
            }
            for (int k = i - 1; k >= 0; --k) {
                if (matrix[k][j] >= val) {
                    --visible;
                    break;
                }
            }
            for (int k = i + 1; k < matrix.size(); ++k) {
                if (matrix[k][j] >= val) {
                    --visible;
                    break;
                }
            }
            if (visible > 0) {
                total_visible += 1;
            }
        }
    }
    std::cout << "visible: " << total_visible;

}

void day8_2()
{
    std::vector<std::vector<char>> matrix;

    std::ifstream file("day8.txt");
    if (file.is_open()) {

        for (std::string line; std::getline(file, line); )
        {
            matrix.push_back({});
            for (char c : line) {
                matrix[matrix.size() - 1].push_back(c);
            }
        }
        file.close();
    }

    int highest_sceenic_score = 0;

    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[i].size(); ++j) {
            char val = matrix[i][j];

            int val1 = 0, val2 = 0, val3 = 0, val4 = 0;

            for (int k = j - 1; k >= 0; --k) { // left
                val1++;
                if (matrix[i][k] >= val) { // higher, therefore not visible
                    break;
                }
            }
            for (int k = j + 1; k < matrix[i].size(); ++k) { // right
                val2++;
                if (matrix[i][k] >= val) {
                    break;
                }
            }
            for (int k = i - 1; k >= 0; --k) {
                val3++;
                if (matrix[k][j] >= val) {
                    break;
                }
            }
            for (int k = i + 1; k < matrix.size(); ++k) {
                val4++;
                if (matrix[k][j] >= val) {
                    break;
                }
            }
            int sceenic_score = val1 * val2 * val3 * val4;
            if (sceenic_score > highest_sceenic_score) {
                highest_sceenic_score = sceenic_score;
            }
        }
    }
    std::cout << "highest: " << highest_sceenic_score;

}